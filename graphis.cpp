#include <Windows.h>
#include <stdexcept>
#include <string>
#include "graphic.h"
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "bitmap.h"

auto& frm = frame::Frame::frm;

namespace graphic::shader {
	unsigned CompileShader(const char* str, const int glsl, const int GL_XXXX_SHADER) {
		auto resource{ FindResourceA(NULL, MAKEINTRESOURCEA(glsl), str) };
		unsigned shader = 0;
		if (resource) {
			int size{ static_cast<int>(SizeofResource(NULL, resource)) };
			auto data{ LockResource(LoadResource(NULL, resource)) };
			auto ptr = static_cast<const char*>(data);
			shader = glCreateShader(GL_XXXX_SHADER);
			glShaderSource(shader, 1, &ptr, &size);
			glCompileShader(shader);

			int success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				char infoLog[512];
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				throw std::invalid_argument{ "Compile shader fail" };
			}
		}
		return shader;
	}
	unsigned id::GetShader(const char* str, int vert, int frag, int geom) {
		unsigned vertex{ CompileShader(str, vert, GL_VERTEX_SHADER) };
		unsigned fragment{ CompileShader(str, frag, GL_FRAGMENT_SHADER) };
		unsigned geometry{ geom ? CompileShader(str, geom, GL_GEOMETRY_SHADER) : 0 };

		auto shader{ glCreateProgram() };
		glAttachShader(shader, vertex);
		glAttachShader(shader, fragment);
		if (geometry)
			glAttachShader(shader, geometry);
		glLinkProgram(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
			throw std::invalid_argument{ "Link " + std::string{ str } + " shader fail" };

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometry)
			glDeleteShader(geometry);

		constexpr std::string_view label{ "texture_" };
		constexpr int count = 16;
		glUseProgram(shader);

		for (int index = 0; index < count; ++index) {
			std::string temp{ std::string{ label } + std::to_string(index) };
			auto location = glGetUniformLocation(shader, temp.data());
			if (-1 != location)
				glUniform1i(location, index);
		}

		return shader;
	}
}
namespace graphic::shape {
	bool Rectangle::Border(const glm::ivec2 pos) const {
		return pos.x >= rectangle.z && pos.x < rectangle.x && pos.y >= rectangle.w && pos.y < rectangle.y;
	}
	glm::ivec2 Rectangle::SetCenter(const glm::ivec2 center) {
		SetRectangle(center, size);
		return center;
	}
	glm::ivec2 Rectangle::SetSize(const glm::ivec2 size) {
		SetRectangle(center, size);
		return size;
	}
	glm::ivec4 Rectangle::SetRectangle(const glm::ivec4 border) {
		int X = (border.x + border.z + 1) >> 1;
		int Y = (border.y + border.w + 1) >> 1;
		int W = border.x - border.z;
		int H = border.y - border.w;
		center = { X,Y };
		size = { W,H };
		this->rectangle = border;
		return border;
	}
	glm::ivec4 Rectangle::SetRectangle(const glm::ivec2 center, const glm::ivec2 size) {
		this->center = center;
		this->size = size;
		int R = center.x + ((size.x + 1) >> 1);
		int U = center.y + ((size.y + 1) >> 1);
		int L = R - size.x;
		int D = U - size.y;
		rectangle = { R, U, L, D };
		return rectangle;
	}
	/*
	glm::vec4 Rectangle::GetTextures(const texture::Texture::Data& texture) const {
		int R = center.x + texture.center.x + ((texture.size.x + 1) >> 1);
		int U = center.y + texture.center.y + ((texture.size.y + 1) >> 1);
		int L = R - texture.size.x;
		int D = U - texture.size.y;
		std::cout << texture.center.x << " " << texture.center.y << " " << texture.size.x << " " << texture.size.y << "\n";
		std::cout << R << " " << U << " " << L << " " << D << "\n";
		return { R, U, L, D };
	}
	*/
}
namespace graphic::layout {
	glm::ivec2 Layout::GetSize(const glm::ivec2 step, const int count) const {
		if (select)
			return { step.x, step.y * count };
		else
			return { step.x * count, step.y };		
	}
	glm::ivec2 Layout::GetPos(const glm::ivec2 pos, const glm::ivec2 size,
		const int count, const int index) const {
		if (select) {
			float H = size.y * count * .5f;
			int Y = H - size.y * (.5f + index);
			return { pos.x, pos.y + Y };
		}
		else {
			float W = size.x * count * .5f;
			int X = size.x * (.5f + index) - W;
			return { pos.x + X, pos.y };
		}
	}
}