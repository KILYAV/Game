#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
		struct Sphere {
		private:
			glm::ivec3 center{ 0,0,0 };
			glm::ivec3 radius{ 0,0,0 };
		public:
			static constexpr size_t count = 1;

			glm::ivec2 GetCenter() const {
				return center;
			}
			glm::ivec2 GetRadius() const {
				return radius;
			}
		};
		struct Rectangle {
		private:
			glm::ivec2 center{ 0,0 };
			glm::ivec2 size{ 0,0 };
			glm::ivec4 rectangle{ 0,0,0,0 };
		public:
			static constexpr size_t count = 1;

			bool Border(const glm::ivec2 pos) const;
			glm::ivec2 GetCenter() const {
				return center;
			}
			glm::ivec2 GetSize() const {
				return size;
			}
			glm::ivec4 GetBorder() const {
				return rectangle;
			}
			glm::ivec2 SetCenter(const glm::ivec2 pos);
			glm::ivec2 SetSize(const glm::ivec2 size);
			glm::ivec4 SetRectangle(const glm::ivec4 border);
			glm::ivec4 SetRectangle(const glm::ivec2 pos, const glm::ivec2 size);

			//glm::vec4 GetTextures(const texture::Texture::Data& texture) const;
		};
	}
	namespace vertex {
		template<class Shape>
		struct Array :
			Shape
		{
			const unsigned VAO;
			const unsigned VBO;
			const unsigned EBO;
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
				}) },
				EBO{ std::invoke([]() {
					unsigned EBO;
					glGenBuffers(1, &EBO);
					return EBO;
				}) }
			{}
			void Bind() const {
				glBindVertexArray(VAO);
				//glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			}
			template<typename... Input_t>
			void LoadVertex(const Input_t... input) const;
		};
		template<class Shape>
		template<typename... Input_t>
		void Array<Shape>::LoadVertex(const Input_t... input) const {
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
					GL_FALSE, count, (void*)offset);
				glEnableVertexAttribArray(index);
				offset += size[index++];
				}), ...);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		using Rectangle = Array<shape::Rectangle>;
	}
}