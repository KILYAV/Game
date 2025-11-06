#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
		struct Rectangle {
		private:
			glm::ivec2 pos{ 0,0 };
			glm::ivec2 size{ 0,0 };
			glm::ivec4 region{ 0,0,0,0 };
		public:
			static constexpr size_t count = 1;

			bool Region(const glm::ivec2 pos) const;
			glm::ivec2 GetPos() const {
				return pos;
			}
			glm::ivec2 SetPos(const glm::ivec2 pos) {
				return this->pos = pos;
			}
			glm::ivec2 GetSize() const {
				return size;
			}
			glm::ivec2 SetSize(const glm::ivec2 size) {
				return this->size = size;
			}
			glm::ivec4 GetRegion() const {
				return region;
			}
			glm::ivec4 SetRegion();

			glm::vec4 GetBorder() const;
			glm::vec4 GetRectangle() const;
			std::vector<glm::vec4> GetTexture(const std::vector<texture::Texture::Data> texture) const;
		private:
			glm::vec4 GetVertex(const glm::ivec2 pos, const glm::ivec2 size_tex,
				const glm::ivec2 size_rect) const;
		};
	}
	namespace vertex {
		template<class Shape>
		struct Array :
			Shape
		{
			const unsigned VAO;
			const unsigned VBO;
			Array() :
				Shape{},
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
			void Bind() const {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
			}
			template<typename... Input_t>
			void LoadVertex(const Input_t... input);
		};
		template<class Shape>
		template<typename... Input_t>
		void Array<Shape>::LoadVertex(const Input_t... input) {
			int index = 0;
			int count = 0;
			int offset = 0;
			int size[sizeof...(Input_t)];
			(std::invoke([&]() {
				if constexpr (std::is_trivial_v<Input_t>)
					count += size[index++] = sizeof(Input_t);
				else
					count += size[index++] = input.size() * sizeof(Input_t::value_type);
				}), ...);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, count, NULL, GL_STATIC_DRAW);

			index = 0;
			(std::invoke([&]() {
				if constexpr (std::is_trivial_v<Input_t>)
					glBufferSubData(GL_ARRAY_BUFFER, offset, size[index], &input);
				else
					glBufferSubData(GL_ARRAY_BUFFER, offset, size[index], input.data());

				glVertexAttribPointer(index, size[index] / sizeof(float), GL_FLOAT,
					GL_FALSE, size[index], (void*)offset);
				glEnableVertexAttribArray(index);
				offset += size[index++];
				}), ...);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		using Rectangle = Array<shape::Rectangle>;
		/*
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
		*/
	}
}