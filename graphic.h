#pragma once
#include <tuple>
#include <array>
#include <vector>
#include <functional>
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "resource.h"
#include "shader.h"
#include "vertex.h"

namespace graphic {
	namespace object {
		template<class Shader, template<class> class Vertex, class... Texture>
		struct Object :
			Shader,
			Vertex<Texture...>,
			Texture...
		{
		protected:
			Object(Vertex<Texture...>::input_t input) :
				Shader{},
				Vertex<Texture...>{ input },
				Texture{}...
			{}
			const void Draw() const;
		};
		template<class Shader, template<class> class Vertex, class... Texture>
		const void Object<Shader, Vertex, Texture...>::Draw() const {
			glUseProgram(Shader::shader);
			glBindVertexArray(Vertex<Texture...>::VAO);
			glBindBuffer(GL_ARRAY_BUFFER, Vertex<Texture...>::VBO);

			unsigned texture{ GL_TEXTURE0 };
			(std::invoke([&]() {
				glActiveTexture(texture++);
				glBindTexture(GL_TEXTURE_2D, Texture::texture.ID);
				}), ...);

			glDrawArrays(GL_POINTS, 0, Vertex<Texture...>::size);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		template<class Texture>
		using Rectangle = Object<shader::Rectangle, vertex::Rectangle, Texture>;
	}
}