#include <Windows.h>
#include <optional>
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "texture.h"
#include "font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace graphic::texture {
	Texture::Texture(const glm::ivec2 size_, const unsigned bit_, const void* data) :
		size{ size_ },
		bit{ bit_ },
		ID{ std::invoke([&]() {
			static float borderColor[] = { 0., 0., 0., 0. };
			int type{ bit == sizeof(bitmap::Red) ? GL_RED : GL_RGB };
			unsigned texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			glPixelStorei(GL_UNPACK_ALIGNMENT, bit);
			glTexImage2D(GL_TEXTURE_2D, 0, type, size.x, size.y, 0, type, GL_UNSIGNED_BYTE, data);
			return texture;
		}) }
	{}
	Texture::Texture(const int ID) :
		Texture{ std::invoke([&]() {
			auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), L"JPG");
			auto source{ LockResource(LoadResource(NULL, resource)) };
			auto size{ SizeofResource(NULL, resource) };
			int width, height, channels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data{ stbi_load_from_memory(static_cast<const unsigned char*>(source),
				size, &width, &height, &channels, 0) };
			if (sizeof(bitmap::Bitmap<bitmap::Red>) == (channels >> 8)) {
				bitmap::Bitmap<bitmap::Red> bitmap{ glm::ivec2{ width, height },
					static_cast<bitmap::Red*>(static_cast<void*>(data)) };
				stbi_image_free(data);
				return Texture{ bitmap };
			}
			if (sizeof(bitmap::Bitmap<bitmap::RGB>) == (channels >> 8)) {
				bitmap::Bitmap<bitmap::RGB> bitmap{ glm::ivec2{ width, height },
					static_cast<bitmap::RGB*>(static_cast<void*>(data)) };
				stbi_image_free(data);
				return Texture{ bitmap };
			}
			})}
	{}
	void Bind(const std::vector<Texture>& texture) {
		unsigned active = GL_TEXTURE0;
		for (auto& elem : texture) {
			elem.Bind(active++);
		}
	}
	glm::ivec2 MaxSize(const std::vector<Texture>& texture) {
		glm::ivec2 max{0,0};
		for (auto& elem : texture) {
			const glm::ivec2 size{ elem.GetSize() };
			max.x = max.x > size.x ? max.x : size.x;
			max.y = max.y > size.y ? max.y : size.y;
		}
		return max;
	}
}