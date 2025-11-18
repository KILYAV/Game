#pragma once
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "shader.h"
#include "vertex.h"

namespace graphic {
	namespace object {
		template<class Shader, class Vertex>
		struct Object :
			Shader,
			Vertex,
			texture::Texture
		{
		public:
			using Texture = texture::Texture;
			Object() :
				Shader{},
				Vertex{},
				Texture{}
			{}
			void Paint() const;
			void LoadVertex(const glm::ivec4 border, const glm::ivec4 texture = {0,0,0,0}) const {
				Vertex::LoadVertex(border, texture);
			}
		};
		template<class Shader, class Vertex>
		void Object<Shader, Vertex>::Paint() const {
			Shader::Bind();
			Vertex::Bind();
			Texture::Bind();

			glDrawArrays(GL_POINTS, 0, Vertex::count);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		using Rectangle = Object<shader::Rectangle, vertex::Rectangle>;
	}
	namespace layout {
		struct Layout {
		private:
			bool select = true;
		public:
			bool SetHorizon() {
				select = true;
			}
			bool SetVertical() {
				select = false;
			}
			glm::ivec2 GetSize(const glm::ivec2 step, const int count) const;
			glm::ivec2 GetPos(const glm::ivec2 pos, const glm::ivec2 size,
				const int count, const int index) const;
		};
	}
}