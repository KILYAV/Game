#pragma once
//#include <vector>
//#include <memory>
#include <type_traits>
//#include "frame.h"
//#include "texture.h"

namespace graphic {
	struct Vertex {
		const unsigned VAO;
		const unsigned VBO;
		const unsigned EBO;
		Vertex() :
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
	template<typename... Input_t>
	void Vertex::LoadVertex(const Input_t... input) const {
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
}