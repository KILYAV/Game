#pragma once
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
		namespace rectangle {
			struct Rectangle {
			private:
				glm::ivec4 region;
			public:
				static constexpr size_t size = 1;

				bool Region(const glm::ivec2 pos) const;
				glm::ivec4 Region(const glm::ivec2 pos, const glm::ivec2 size);
				glm::ivec4 Region() const {
					return region;
				}

				template<size_t count>
				using input_t = std::pair<std::pair<glm::ivec2, glm::ivec2>, std::array<glm::ivec2, count * 2>>;
				template<size_t count>
				using output_t = std::pair<std::pair<glm::vec4, glm::vec4>, std::array<glm::vec4, count>>;
				template<size_t count>
				output_t<count> Get(const input_t<count>& input);
				std::array<glm::vec4, 2> GetPair(const glm::ivec2 pos, const glm::ivec2 size);
			private:
				glm::vec4 GetBorder(const glm::ivec4 region) const;
				glm::vec4 GetRectangle(const glm::ivec4 region) const;
				glm::vec4 GetTexture(const glm::ivec2 pos, const glm::ivec2 size_tex,
					const glm::ivec2 size_rect) const;
			};
			template<size_t count>
			Rectangle::output_t<count> Rectangle::Get(const input_t<count>& input) {
				output_t<count> result;

				auto& pos{ input.first.first };
				auto& size{ input.first.second };
				auto& border{ result.first.first };
				auto& rectangle{ result.first.second };

				region = Region(pos, size);
				border = GetBorder(Region());
				rectangle = GetRectangle(Region());

				if constexpr (count) {
					for (int index = 0; index < count; ++index) {
						auto& pos{ input.second[index * 2] };
						auto& size{ input.second[index * 2 + 1] };
						result.second[index] = GetTexture(pos, size, input.first.second);
					}
				}
				return result;
			}
		}
	}
	namespace vertex {
		namespace base {
			struct Array {
				const unsigned VAO;
				const unsigned VBO;
				Array();
			protected:
				void LoadVertex(const std::array<glm::vec4, 2> input);
			};
		}
		template<class Shape, class... Texture>
		struct Array :
			base::Array,
			Shape
		{
			using input_t = Shape::template input_t<sizeof...(Texture)>;
		protected:
			Array();
			Array(const glm::ivec2 pos, const glm::ivec2 size);
			Array(const input_t& input);
		protected:
			void LoadVertex(const input_t& input);
			void Bind() const {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
			}
		};
		template<class Shape, class... Texture>
		Array<Shape, Texture...>::Array() :
			base::Array{},
			Shape{}
		{}
		template<class Shape, class... Texture>
		Array<Shape, Texture...>::Array(const glm::ivec2 pos, glm::ivec2 size) :
			base::Array{},
			Shape{}
		{
			base::Array::LoadVertex(Shape::GetPair(pos, size));
		}
		template<class Shape, class... Texture>
		Array<Shape, Texture...>::Array(const input_t& input) :
			base::Array{},
			Shape{}
		{
			LoadVertex(input);
		}
		template<class Shape, class... Texture>
		void Array<Shape, Texture...>::LoadVertex(const input_t& input) {
			using array_t = Shape::template output_t<sizeof...(Texture)>;
			array_t array{this->Shape::Get<sizeof...(Texture)>(input)};

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(array_t), &array, GL_STATIC_DRAW);

			unsigned index = 0;
			unsigned offset = 0;
			glVertexAttribPointer(index, sizeof(glm::ivec4) / sizeof(float), GL_FLOAT, GL_FALSE,
				sizeof(glm::ivec4), (void*)offset);
			offset += sizeof(glm::ivec4);
			glEnableVertexAttribArray(index++);
			glVertexAttribPointer(index, sizeof(glm::ivec4) / sizeof(float), GL_FLOAT, GL_FALSE,
				sizeof(glm::ivec4), (void*)offset);
			offset += sizeof(glm::ivec4);
			glEnableVertexAttribArray(index++);

			for (unsigned max = sizeof...(Texture) + 2; index < max; ++index) {
				glVertexAttribPointer(index, sizeof(glm::vec4) / sizeof(float),
					GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)offset);
				glEnableVertexAttribArray(index);
				offset += sizeof(glm::vec4);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		template<class... Texture>
		using Rectangle = Array<shape::rectangle::Rectangle, Texture...>;

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
}