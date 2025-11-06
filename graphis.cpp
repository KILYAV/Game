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
	auto& frm = frame::Frame::frm;

	namespace shader {
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
			unsigned vertex{ CompileShader(str, vert, GL_VERTEX_SHADER)};
			unsigned fragment{ CompileShader(str, frag, GL_FRAGMENT_SHADER)};
			unsigned geometry{ geom ? CompileShader(str, geom, GL_GEOMETRY_SHADER) : 0};

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
			return shader;
		}
	}
	namespace uniform {
		namespace block {
			unsigned GetBlock(const unsigned ranges, const unsigned size) {
				unsigned block = 0;
				glGenBuffers(1, &block);

				glBindBuffer(GL_UNIFORM_BUFFER, block);
				glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				glBindBufferRange(GL_UNIFORM_BUFFER, ranges, block, 0, size);
				return block;
			}
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
			glPixelStorei(GL_UNPACK_ALIGNMENT, align ? sizeof(bitmap::RGB) : sizeof(bitmap::Red));
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, align ? GL_RGB : GL_RED, size.x, size.y, 0,
				align ? GL_RGB : GL_RED, GL_UNSIGNED_BYTE, data);
			return texture;
		}
		Texture::Texture(const std::vector<std::wstring> labels) :
			texture{ std::invoke([&]() {
				std::vector<Data> texture;
				for (auto& label : labels) {
					auto bitmap{ font::Font::fnt.GetBitMap(label) };
					texture.emplace_back(
						LoadTexture(bitmap.Data(), bitmap.GetSize(), false), bitmap.GetSize());
				}
				return texture;
				}) }
		{}
		void Texture::AddTexture(const int ID) {
			auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), L"JPG");
			auto source{ LockResource(LoadResource(NULL, resource)) };
			auto size{ SizeofResource(NULL, resource) };
			int width, height, channels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data{ stbi_load_from_memory(static_cast<const unsigned char*>(source),
					size, &width, &height, &channels, 0) };
			texture.emplace_back(LoadTexture(data, glm::ivec2{ width, height }),
				glm::ivec2{ 0,0 }, glm::ivec2{ width, height }, glm::ivec2{ 1,1 });
		}
		void Texture::AddTexture(const wchar_t* label) {
			auto bitmap{ font::Font::fnt.GetBitMap(label) };
			texture.emplace_back(LoadTexture(bitmap.Data(), bitmap.GetSize(), false),
				glm::ivec2{ 0,0 }, bitmap.GetSize(), glm::ivec2{ 1,1 });
		}
		void Texture::Bind() const {
			unsigned numb{ GL_TEXTURE0 };
			for (auto& text : texture) {
				glActiveTexture(numb++);
				glBindTexture(GL_TEXTURE_2D, text.ID);
			}
		}
		glm::ivec2 Texture::MaxSize() const {
			glm::ivec2 max{ 0,0 };
			for (auto& [ID,pos,size,scale] : texture) {
				max.x = max.x < size.x ? size.x : max.x;
				max.y = max.y < size.y ? size.y : max.y;
			}
			return max;
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
				auto bitmap{ font::Font::fnt.GetBitMap(label) };
				return { LoadTexture(bitmap.Data(), bitmap.GetSize(), false), bitmap.GetSize() };
			}
		}
	}
	namespace shape {
		bool Rectangle::Region(const glm::ivec2 pos) const {
			return pos.x >= region.w && pos.x < region.y && pos.y >= region.z && pos.y < region.x;
		}
		glm::ivec4 Rectangle::SetRegion() {
			int R{ pos.x + ((size.x + 1) >> 1) };
			int U{ pos.y + ((size.y + 1) >> 1) };
			int L{ R - size.x };
			int D{ U - size.y };
			return region = { U, R, D, L };
		}
		glm::vec4 Rectangle::GetBorder() const {
			auto& center{ frame::Frame::frm.Size().center };
			float U{ static_cast<float>(region.x + center.y) };
			float R{ static_cast<float>(region.y + center.x) };
			float D{ static_cast<float>(region.z + center.y) };
			float L{ static_cast<float>(region.w + center.x) };
			return { U, R, D, L };
		}
		glm::vec4 Rectangle::GetRectangle() const {
			auto& pixel{ frame::Frame::frm.Size().pixel };
			float U{ region.x * pixel.y * 2 };
			float R{ region.y * pixel.x * 2 };
			float D{ region.z * pixel.y * 2 };
			float L{ region.w * pixel.x * 2 };
			return { U, R, D, L };
		}
		std::vector<glm::vec4> Rectangle::GetTexture(const std::vector<texture::Texture::Data> texture) const {
			std::vector<glm::vec4> result;
			result.resize(texture.size());
			for (int index = 0, max = texture.size(); index < max; ++index) {
				result[index] = GetVertex(texture[index].pos, texture[index].size, size);
			};
			return result;
		}
		glm::vec4 Rectangle::GetVertex(const glm::ivec2 pos, const glm::ivec2 texture,
			const glm::ivec2 rectangle) const {
			auto& pixel{ frame::Frame::frm.Size().pixel };
			float X{ pos.x * pixel.x };
			float Y{ pos.y * pixel.y };
			float W{ rectangle.x * .5f / texture.x };
			float H{ rectangle.y * .5f / texture.y };
			return { .5f + H + Y, .5f + W + X, .5f - H + Y, .5f - W + X };
		}
	}
	namespace vertex {
		/*
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
		*/
	}
}
