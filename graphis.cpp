#include <Windows.h>
#include <stdexcept>
#include "graphic.h"
#include "setting.h"
#include "frame.h"
#include "bitmap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace graphic {
	namespace shader {
		unsigned CompileShader(const int glsl, const wchar_t* type, const int GL_XXXX_SHADER) {
			auto resource{ FindResource(NULL, MAKEINTRESOURCE(glsl), type) };
			unsigned shader = 0;
			if (resource) {
				auto data{ LockResource(LoadResource(NULL, resource)) };
				auto ptr = static_cast<const char*>(data);
				shader = glCreateShader(GL_XXXX_SHADER);
				glShaderSource(shader, 1, &ptr, NULL);
				glCompileShader(shader);
				
				int success;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
					throw std::invalid_argument{"Compile shader fail"};
			}
			return shader;
		}
		unsigned GetShader(const wchar_t* type, int vert, int frag, int geom) {
			unsigned vertex{ CompileShader(vert, type, GL_VERTEX_SHADER) };
			unsigned fragment{ CompileShader(frag, type, GL_FRAGMENT_SHADER) };
			unsigned geometry = 0;
			if (geom)
				geometry = CompileShader(frag, type, GL_GEOMETRY_SHADER);

			auto shader{ glCreateProgram() };
			glAttachShader(shader, vertex);
			glAttachShader(shader, fragment);
			if (geometry)
				glAttachShader(shader, geometry);
			glLinkProgram(shader);

			int success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
				throw std::invalid_argument{ "Link shader fail" };

			glDeleteShader(vertex);
			glDeleteShader(fragment);
			if (geometry)
				glDeleteShader(geometry);
			return shader;
		}
	}
	namespace texture {
		Data Data::GetData(const int ID)  {
			unsigned texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 0., 0., 0., 0. };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), L"JPG");
			auto source{ LockResource(LoadResource(NULL, resource)) };
			auto size{ SizeofResource(NULL, resource) };
			int width, height, channels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data
				= stbi_load_from_memory(static_cast<const unsigned char*>(source), size, &width, &height, &channels, 0);
			glPixelStorei(GL_UNPACK_ALIGNMENT, sizeof(bitmap::RGB));
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			return { texture, height, width };
		}
	}
	namespace vertex {
		Rectangle::output Rectangle::GetRectangle(input rectangle) {
			float right{ rectangle.x / frm.size.floating.x };
			float top{ rectangle.y / frm.size.floating.y };
			float down{ -top };
			float left{ -right };
			return output{
				tuple{ { top, left   }, { 0., 1. } },
				tuple{ { top, right  }, { 1., 1. } },
				tuple{ { down, left  }, { 0., 0. } },
				tuple{ { down, right }, { 1., 0. } }
			};
		};
		Element::Element(const std::vector<unsigned>& indices) :
			EBO{ std::invoke([]() {
				unsigned EBO;
				glGenBuffers(1, &EBO);
				return EBO;
			}) },
			size{ static_cast<unsigned>(indices.size()) }
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned),
				static_cast<const char*>(static_cast<const void*>(indices.data())), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	/*
	std::map<const char*, unsigned> uniforms::GetUniforms(
		const unsigned shader, const std::vector<const char*> labels) {
		std::map<const char*, unsigned> uniforms;
		for (const auto& ptr : labels) {
			uniforms.emplace(ptr, glGetUniformLocation(shader, ptr));
		}
		return uniforms;
	}
	bool uniforms::Rectangle::operator== (const glm::ivec2 pos) const {
		return  value.x < pos.x && pos.x < value.z && value.y < pos.y && pos.y < value.w;
	}
	uniforms::Rectangle uniforms::Rectangle::operator- (const int border) const {
		return glm::vec4{ value.x + border, value.y + border, value.z - border, value.w - border };
	}
	*/
}
