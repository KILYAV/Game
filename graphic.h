#pragma once
#include <map>
#include <tuple>
#include <array>
#include <vector>
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"

#include "resource.h"

namespace graphic {
	namespace label {
		static constexpr wchar_t backdrop[] = L"BACKDROP";
		static constexpr std::wstring_view button{ L"BUTTON" };
	}
	namespace shader {
		unsigned GetShader(const wchar_t* type, int vert, int frag, int geom = 0);
		template<const wchar_t* type, int vert, int frag, int geom = 0>
		class Shader {
		public:
			inline static const unsigned shader{ GetShader(type, vert, frag, geom) };
		protected:
			static void BindShader() {
				glUseProgram(shader);
			}
		};
		using Backdrop = Shader<label::backdrop, IDR_BACKDROP_SHADER_VERT, IDR_BACKDROP_SHADER_FRAC>;
	}
	namespace texture {
		struct Data {
			unsigned ID;
			int h;
			int w;
			static Data GetData(const int ID);
		};
		template<int ID>
		class Texture {
		public:
			inline static const Data data{ Data::GetData(ID) };
		protected:
			static void BindTexture() {
				glBindTexture(GL_TEXTURE_2D, texture);
			}
		};
		using Backdrop = Texture<IDR_BACKDROP_JPG>;
	}
	namespace vertex {
		class Rectangle {
		public:
			static constexpr size_t size = 4;
			using input = const glm::vec2;
			using tuple = std::tuple<glm::vec2, glm::vec2>;
			using output = std::array<tuple, size>;
		protected:
			static output GetRectangle(input rectangle);
		};
		template<class Shape>
		class Array :
			public Shape
		{
		public:
			const unsigned VAO;
			const unsigned VBO;
		protected:
			Array(Shape::input array);
			void BindArray() const {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
			}
		private:
			template<size_t... In>
			void LoadVertex(const char* data, std::index_sequence<In...>) const;
		};
		template<class Shape>
		Array<Shape>::Array(Shape::input input) :
			VAO{ std::invoke([]() {
				unsigned VAO;
				glGenVertexArrays(1, &VAO);
				return VAO;
			}) },
			VBO{ std::invoke([]() {
				unsigned VBO;
				glGenBuffers(1, &VBO);
				return VBO;
			}) }
		{
			LoadVertex(static_cast<const char*>(static_cast<const void*>(Shape::GetRectangle(input).data())),
				std::make_index_sequence<std::tuple_size_v<Shape::tuple>>{});
		}
		template<class Shape>
		template<size_t... In>
		void Array<Shape>::LoadVertex(const char* data, std::index_sequence<In...>) const {
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Shape::output), data, GL_STATIC_DRAW);

			unsigned offset = 0;
			(std::invoke([&]() {
				glVertexAttribPointer(In, sizeof(std::tuple_element_t<In, Shape::tuple>) / sizeof(float),
					GL_FLOAT, GL_FALSE, sizeof(Shape::tuple), (void*)offset);
				glEnableVertexAttribArray(In);
				offset += sizeof(std::tuple_element_t<In, Shape::tuple>);
				}), ...);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		class Element {
		public:
			const unsigned EBO;
			const unsigned size;
		protected:
			Element(const std::vector<unsigned>& indices = {});
			void BindElement() const {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			}
		};
	}
	namespace shape {
		template<class Shader, class Texture, class Array>
		class Shape :
			Shader,
			Texture,
			Array
		{
		public:
			Shape(Array::input input) :
				Shader{},
				Texture{},
				Array{ input }
			{}
		//protected:
			static unsigned Type() { return 0; };
			const void BindShape() const;
		private:
			virtual void BindUniform() const {};
		};
		template<class Shader, class Texture, class Array>
		const void Shape<Shader, Texture, Array>::BindShape() const {
			Shader::BindShader();
			Texture::BindTexture();
			Array::BindArray();
		};
		using Backdrop = Shape<shader::Backdrop, texture::Backdrop, vertex::Array<vertex::Rectangle>>;
	}
	/*
	namespace uniforms {
		std::map<const char*, unsigned> GetUniforms(const unsigned shader, const std::vector<const char*> labels);
		struct Bool {
			using type = bool;
			bool value = true;

			Bool(bool select = true) { value = select; };
		};
		struct Flash : Bool {
			static constexpr const char* label{ "flash" };
		};
		struct Rectangle {
			using type = glm::vec4;
			glm::vec4 value;

			Rectangle(const glm::vec4 rectangle = {}) :
				value{ rectangle }
			{}
			Rectangle(const glm::ivec2 center, const glm::ivec2 size) :
				value{ glm::Rectangle(center, size) }
			{}

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
		public Interface,
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
			Interface{ vertices, indices, bitmap },
			uniforms::Window{ type... }
		{}
		const Interface& BindShader() const {
			glUseProgram(shader.shader);
			return *this;
		}
		const Interface& BindUniform() const {
			uniforms::Window::BindUniform();
			return *this;
		}
		unsigned Type() const {
			return GL_TRIANGLE_STRIP;
		}
	};
	*/
}