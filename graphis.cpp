#include <Windows.h>
#include <stdexcept>
#include "graphic.h"
#include "setting.h"
#include "frame.h"
#include "font.h"
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
					throw std::invalid_argument{ "Compile shader fail" };
			}
			return shader;
		}
		unsigned id::GetShader(const wchar_t* type, int vert, int frag, int geom) {
			unsigned vertex{ CompileShader(vert, type, GL_VERTEX_SHADER) };
			unsigned fragment{ CompileShader(frag, type, GL_FRAGMENT_SHADER) };
			unsigned geometry = 0;
			if (geom)
				geometry = CompileShader(geom, type, GL_GEOMETRY_SHADER);

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
		unsigned LoadTexture(unsigned char* data, const glm::ivec2 size, const bool align = true) {
			unsigned texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 0., 0., 0., 0. };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, align ? sizeof(bitmap::RGB): sizeof(bitmap::Red));
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, align ? GL_RGB : GL_RED, size.x, size.y, 0,
				align ? GL_RGB : GL_RED, GL_UNSIGNED_BYTE, data);
			return texture;
		}
		namespace id {
			data::Texture GetTexture(const int ID) {
				auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), L"JPG");
				auto source{ LockResource(LoadResource(NULL, resource)) };
				auto size{ SizeofResource(NULL, resource) };
				int width, height, channels;
				stbi_set_flip_vertically_on_load(true);
				unsigned char* data
					= stbi_load_from_memory(static_cast<const unsigned char*>(source), size, &width, &height, &channels, 0);
				return { LoadTexture(data, { width, height }), { width, height } };
			}
		}
		namespace str {
			data::Texture GetTexture(const wchar_t* label) {
				auto bitmap{ fnt.GetBitMap(label) };
				return { LoadTexture(bitmap.Data(), bitmap.GetSize(), false), bitmap.GetSize() };
			}
		}
	}
	namespace shape {
		namespace rectangle {
			glm::vec4 GetRegion(const glm::ivec2 pos, const glm::ivec2 size) {
				float X{ pos.x * frm.size.pixel.x };
				float Y{ pos.y * frm.size.pixel.y };
				float W{ size.x * frm.size.pixel.x };
				float H{ size.y * frm.size.pixel.y };
				return { Y + H, X + W, Y - H, X - W };
			}
			glm::vec4 GetTexture(const glm::ivec2 pos, const glm::ivec2 size_tex, const glm::ivec2 size_rect) {
				float X{ pos.x * frm.size.pixel.x };
				float Y{ pos.y * frm.size.pixel.y };
				float W{ size_tex.x / size_rect.x * .5f };
				float H{ size_tex.y / size_rect.y * .5f };
				return { .5 + H + Y, .5 + W + X, .5 - H + Y, .5 - W + X };
			}
		}
	}
	namespace vertex {
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
}
