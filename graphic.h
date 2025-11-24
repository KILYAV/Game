#pragma once
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "shader.h"
#include "vertex.h"
#include "texture.h"

namespace graphic {
	template<class Shader>
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
		void LoadVertex(const glm::ivec4 border, const glm::ivec4 texture = { 0,0,0,0 }) const {
			Vertex::LoadVertex(border, texture);
		}
	};

	struct Rectangle :
		Object<shader::Rectangle>
	{
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
		glm::ivec2 GetSize() {
			return size;
		}
		glm::ivec4 GetBorder() const {
			return rectangle;
		}
		glm::ivec2 SetCenter(const glm::ivec2 pos);
		glm::ivec2 SetSize(const glm::ivec2 size);
		glm::ivec4 SetRectangle(const glm::ivec4 border);
		glm::ivec4 SetRectangle(const glm::ivec2 pos, const glm::ivec2 size);

		void Paint() const;
	};

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