#pragma once
#include "frame.h"
#include "texture.h"

namespace graphic {
	namespace shape {
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

			template<typename... Input_t>
			std::array<glm::vec4, 2 + sizeof...(Input_t)> Get(const glm::ivec2 pos,
				const glm::ivec2 size, const Input_t... input);
		private:
			glm::vec4 GetBorder(const glm::ivec4 region) const;
			glm::vec4 GetRectangle(const glm::ivec4 region) const;
			glm::vec4 GetTexture(const glm::ivec2 pos, const glm::ivec2 size_tex,
				const glm::ivec2 size_rect) const;
		};
		template<typename... Input_t>
		std::array<glm::vec4, 2 + sizeof...(Input_t)> Rectangle::Get(const glm::ivec2 pos,
			const glm::ivec2 size, const Input_t... input) {
			std::array<glm::vec4, sizeof...(Input_t) + 2> array;

			region = Region(pos, size);
			auto border = GetBorder(Region());
			auto rectangle = GetRectangle(Region());

			int index = 0;
			array[index++] = border;
			array[index++] = rectangle;

			(std::invoke([&](const auto pos, const auto texture, const auto rectangle) {
				array[index++] = GetTexture(pos, texture, rectangle);
				}, input.first, input.second, size), ...);
			return array;
		}
	}
	namespace vertex {
		template<class Shape>
		struct Array :
			Shape
		{
			const unsigned VAO;
			const unsigned VBO;
		protected:
			Array() :
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
			template<typename pos_t, typename size_t, typename... Input_t>
			Array(const pos_t pos, const size_t size, const Input_t... input) :
				Array{}
			{
				LoadVertex(pos, size, input...);
			}
			void Bind() const {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
			}
			template<typename pos_t, typename size_t, typename... Input_t>
			void LoadVertex(const pos_t pos, const size_t size, const Input_t... input);
		};
		template<class Shape>
		template<typename pos_t, typename size_t, typename... Input_t>
		void Array<Shape>::LoadVertex(const pos_t pos, const size_t size, const Input_t... input) {
			auto array{ Shape::Get(pos, size, input...) };

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(array), &array, GL_STATIC_DRAW);

			for (unsigned index = 0, offset = 0, max = sizeof...(Input_t) + 2; index < max; ++index) {
				glVertexAttribPointer(index, sizeof(glm::vec4) / sizeof(float),
					GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)offset);
				glEnableVertexAttribArray(index);
				offset += sizeof(glm::vec4);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		using Rectangle = Array<shape::Rectangle>;

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