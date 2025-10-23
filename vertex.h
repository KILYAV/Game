#pragma once
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
		namespace rectangle {
			struct Rectangle {
				static constexpr size_t size = 1;

				glm::ivec4 Region() const {
					return region;
				}
				bool Region(const glm::ivec2 pos) const {
					return pos.x >= region.w && pos.x < region.y && pos.y >= region.z && pos.y < region.x;
				}
				template<size_t count>
				std::array<glm::vec4, count + 1> Get(const std::array<glm::ivec2, (count + 1) * 2>& input);
			private:
				glm::ivec4 GetRegion(const glm::ivec2 pos, const glm::ivec2 size);
				glm::vec4 GetRegion(const glm::ivec2 pos, const glm::ivec4 region);
				glm::vec4 GetTexture(const glm::ivec2 pos, const glm::ivec2 size_tex,
					const glm::ivec2 size_rect);
				glm::ivec4 region;
			};
			template<size_t count>
			std::array<glm::vec4, count + 1> Rectangle::Get(
				const std::array<glm::ivec2, (count + 1) * 2>& input) {
				region = GetRegion(input[0], input[1]);

				std::array<glm::vec4, count + 1> result;
				result[0] = GetRegion(input[0], region);
				for (int index = 1, max = count + 1; index < max; ++index) {
					result[index] = GetTexture(input[index * 2], input[index * 2 + 1], input[1]);
				}
				return result;
			}
		}
	}
	namespace vertex {
		template<class Shape, class... Texture>
		struct Array :
			Shape
		{
			const unsigned VAO;
			const unsigned VBO;

			using input_t = std::array<glm::ivec2, (sizeof...(Texture) + 1) * 2>;
		protected:
			Array();
			Array(const input_t& input);
		protected:
			void LoadVertex(const input_t& input);
		};
		template<class Shape, class... Texture>
		Array<Shape, Texture...>::Array() :
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
		{}
		template<class Shape, class... Texture>
		Array<Shape, Texture...>::Array(const input_t& input) :
			Array{}
		{
			LoadVertex(input);
		}
		template<class Shape, class... Texture>
		void Array<Shape, Texture...>::LoadVertex(const input_t& input) {
			using array_t = std::array<glm::vec4, sizeof...(Texture) + 1>;
			array_t array{ this->Shape::Get<sizeof...(Texture)>(input)};

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(array_t), array.data(), GL_STATIC_DRAW);

			for (unsigned index = 0, max = sizeof...(Texture) + 1, offset = 0; index < max; ++index) {
				glVertexAttribPointer(index, sizeof(array_t::value_type) / sizeof(float),
					GL_FLOAT, GL_FALSE, sizeof(array_t), (void*)offset);
				glEnableVertexAttribArray(index);
				offset += sizeof(array_t::value_type);
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