#pragma once
#include <map>
#include <tuple>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resource.h"
#include "bitmap.h"

namespace graphics {
	namespace vertex {
		class Array {
		public:
			const unsigned VAO;
			const unsigned VBO;
		private:
			unsigned size = 0;
		public:
			unsigned Size() const {
				return size;
			}
		protected:
			Array();
			template<class Tuple>
			Array(const std::vector<Tuple>& vector) {
				size = static_cast<unsigned>(vector.size());
				LoadVertices(vector);
			}
			void BindBuffer() const;
			template<class Tuple>
			void LoadVertices(const std::vector<Tuple>& vector) {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(Tuple),
					static_cast<const char*>(static_cast<const void*>(vector.data())), GL_STATIC_DRAW);
				VerticesAttribPointers<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				size = static_cast<unsigned>(vector.size());
			}
		private:
			template<class Tuple, size_t... In>
			void VerticesAttribPointers(std::index_sequence<In...>) const {
				(VertexAttribPointer(In, sizeof(std::tuple_element_t<In, Tuple>) / sizeof(float),
					GL_FLOAT, sizeof(Tuple), Offset<Tuple>(std::make_index_sequence<In>{})), ...);
			}
			template<class Tuple, size_t... In>
			constexpr unsigned Offset(std::index_sequence<In...>) const {
				unsigned offset = 0;
				((offset += sizeof(std::tuple_element_t<In, Tuple>)), ...);
				return offset;
			}
			void VertexAttribPointer(unsigned attr, unsigned count, unsigned type, unsigned size, unsigned offset) const;
		public:
			static std::vector<std::tuple<glm::vec2, glm::vec2>> GetRectangle(const glm::ivec4& rectangl);
		};
		class Element {
		public:
			const unsigned EBO;
		private:
			unsigned size = 0;
		public:
			unsigned Size() const {
				return size;
			}
		protected:
			Element(const std::vector<unsigned>& indices = {});
			void BindBuffer() const;
			void LoadElement(const std::vector<unsigned>& indices);
		};
	}
	class Texture {
	public:
		const unsigned texture; 
	protected:
		Texture();
		template<class Pixel>
		Texture(const bitmap::BitMap<Pixel>& bitmap) :
			Texture()
		{
			LoadTexture(bitmap);
		}
		template<typename Pixel>
		const Texture& LoadTexture(const bitmap::BitMap<Pixel>& bitmap) const {
			if (false == bitmap.Empty()) {
				glPixelStorei(GL_UNPACK_ALIGNMENT, sizeof(Pixel));
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D, 0, Pixel::GL_XXX, bitmap.GetSize().x, bitmap.GetSize().y, 0, Pixel::GL_XXX, GL_UNSIGNED_BYTE, bitmap.Data());
			}
			return *this;
		}
		unsigned BindTexture() const;
	};
	class Graphics :
		public vertex::Array,
		public vertex::Element,
		public Texture
	{
	public:
		Graphics() = default;
		template<class Tuple, class Pixel>
		Graphics(
			const std::vector<Tuple>& vertices,
			//const std::vector<unsigned>& indices,
			const bitmap::BitMap<Pixel>& bitmap
		) :
			Array{ vertices },
			Element{ /*indices*/ },
			Texture{ bitmap }
		{}
		const Graphics& BindBuffer() const;
		const Graphics& BindTexture() const;
		virtual const Graphics& BindShader() const = 0;
		virtual const Graphics& BindUniform() const = 0;
		virtual unsigned Type() const = 0;
	};
	namespace shader {
		namespace label {
			static constexpr const wchar_t button[7] = L"BUTTON";
		}
	}
	class Shader {
	public:
		const unsigned shader;
		static unsigned LoadShader(const wchar_t* type, int vert, int frag, int geom = 0) {
			using namespace shader;
			return LinkShader(
				CompileShader(vert, type, GL_VERTEX_SHADER),
				CompileShader(frag, type, GL_FRAGMENT_SHADER)
			);
		}
	private:
		static unsigned CompileShader(const int glsl, const wchar_t* type, const int GL_XXXX_SHADER);
		static unsigned LinkShader(const int vert, const int frag, const int geom = 0);
	};
	namespace uniforms {
		std::map<const char*, unsigned> GetUniforms(const unsigned shader, const std::vector<const char*> labels);
		struct Bool {
			using type = bool;
			bool value = true;

			Bool(bool select = true) { value = select; };
		};
		struct Flash : Bool {
			static constexpr const char* label{ "flash" }; };
		struct Rectangle {
			using type = glm::vec4;
			glm::vec4 value;

			Rectangle(const glm::vec4 rectangle) :
				value{ rectangle }
			{}
			Rectangle(const glm::ivec2 center, const glm::ivec2 size);

			bool operator== (const glm::ivec2) const;
			Rectangle operator- (const int border) const;

			static constexpr const char* label{ "border" };
		};
	}
	template<class... Type>
	class Uniform :
		protected Type...
	{
	private:
		static const std::map<const char*, unsigned> uniforms;
	public:
		Uniform() = default;
		Uniform(const Type&... type) :
			Type{ type }...
		{}
		void BindUniform() const {
			Binds(std::make_index_sequence<sizeof...(Type)>{});
		};
	private:
		template<size_t... Indices>
		void Binds(std::index_sequence<Indices...>) const {
			(Bind<std::tuple_element_t<Indices, std::tuple<Type...>>>(
				static_cast<const std::tuple_element_t<Indices, std::tuple<Type...>>&>(*this).value), ...);
		}
		template<class T, class U>
		void Bind(const U& value) const;
		template<>
		void Bind<uniforms::Rectangle>(const glm::vec4& value) const {
			glUniform4f(uniforms.find(uniforms::Rectangle::label)->second, value.x, value.y, value.z, value.w);
		}
		template<>
		void Bind<uniforms::Flash>(const bool& value) const {
			glUniform1i(uniforms.find(uniforms::Flash::label)->second, value);
		}
	public:
		static std::map<const char*, unsigned> GetUniforms(const unsigned shader) {
			return uniforms::GetUniforms(shader, std::invoke([]() {
				std::vector<const char*> labels;
				GetLabels<Type...>(labels);
				return labels;
				}));
		}
	private:
		template<class... Type>
		static void GetLabels(std::vector<const char*>& labels) {
			(GetLabel<Type>(labels), ...);
		}
		template<class Type>
		static void GetLabel(std::vector<const char*>& labels) {
			labels.push_back(Type::label);
		}
	};
	namespace uniforms {
		using Window = Uniform<uniforms::Flash, uniforms::Rectangle>;
	}
	class Window :
		public Graphics,
		public uniforms::Window
	{
	public:
		static const Shader shader;

		Window() = default;
		template<class Tuple, class Pixel, class... Type>
		Window(
			const std::vector<Tuple> vertices,
			//const std::vector<unsigned> indices,
			const bitmap::BitMap<Pixel> bitmap,
			const Type&... type
		) :
			Graphics{ vertices, /*indices,*/ bitmap},
			uniforms::Window{ type... }
		{}
		const Graphics& BindShader() const {
			glUseProgram(shader.shader);
			return *this;
		}
		const Graphics& BindUniform() const {
			uniforms::Window::BindUniform();
			return *this;
		}
		unsigned Type() const {
			return GL_TRIANGLE_STRIP;
		}
	};
}
