#include <Windows.h>
#include <optional>
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "texture.h"
#include "font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace graphic::texture;

unsigned LoadTexture(const void* data, const glm::ivec2 size, const bool align = true) {
	static float borderColor[] = { 0., 0., 0., 0. };
	int RG_XXX{ align ? GL_RGB : GL_RED };
	int RG_SIZE{ align ? sizeof(bitmap::RGB) : sizeof(bitmap::Red) };
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glPixelStorei(GL_UNPACK_ALIGNMENT, RG_SIZE);
	glTexImage2D(GL_TEXTURE_2D, 0, RG_XXX, size.x, size.y, 0, RG_XXX, GL_UNSIGNED_BYTE, data);
	return texture;
}
unsigned LoadTexture(bitmap::BitMap<bitmap::Red>& bitmap) {
	return LoadTexture(bitmap.Data(), bitmap.GetSize(), false);
}
unsigned LoadTexture(bitmap::BitMap<bitmap::RGB>& bitmap) {
	return LoadTexture(bitmap.Data(), bitmap.GetSize(), true);
}
Texture& Texture::AddTexture(const int ID) {
	auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), L"JPG");
	auto source{ LockResource(LoadResource(NULL, resource)) };
	auto size{ SizeofResource(NULL, resource) };
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data{ stbi_load_from_memory(static_cast<const unsigned char*>(source),
		size, &width, &height, &channels, 0) };
	auto id{ LoadTexture(data, glm::ivec2{ width, height }) };
	stbi_image_free(data);
	texture.push_back({ glm::ivec2{ 0,0 }, glm::ivec2{ width, height }, id });
	return *this;
}
Texture& Texture::AddTexture(const wchar_t* label) {
	auto bitmap{ font::Font::fnt.GetBitMap(label) };
	texture.push_back({ glm::ivec2{ 0,0 }, bitmap.GetSize(), LoadTexture(bitmap) });
	return *this;
}
Texture& Texture::AddTexture(const Texture& other) {
	for (auto& elem : other.texture) {
		texture.push_back(elem);
	}
	return *this;
}
bool Texture::Bind(const unsigned active) const {
	if (texture.size()) {
		glActiveTexture(active);
		glBindTexture(GL_TEXTURE_2D, texture[index].ID);
		return true;
	}
	else
		return false;
}
const Texture::Data& Texture::GetTexture() const {
	static const Data data{};
	if (texture.size())
		return texture[index];
	else
		return data;
}
void Texture::SetCenter(const glm::ivec2 pos) {
	for (auto& data : texture) {
		data.center = pos;
	}
}
glm::ivec2 Texture::GetSize() const {
	glm::ivec2 max{ 0,0 };
	for (auto& [center, size, ID] : texture) {
		max.x = max.x > size.x ? max.x : size.x;
		max.y = max.y > size.y ? max.y : size.y;
	}
	return max;
}
Scroll& Scroll::AddVector(std::vector<std::wstring>&& string) {
	std::vector<bitmap::BitMap<bitmap::Red>> vector;
	for (auto& elem : string) {
		vector.push_back(font::Font::fnt.GetBitMap(elem));
	}
	scroll.push_back(std::move(vector));
	return *this;
}
Texture Scroll::Release() {
	bitmap::BitMap<bitmap::Red> left{ font::Font::fnt.GetBitMap(L"<") };
	bitmap::BitMap<bitmap::Red> center{ font::Font::fnt.GetBitMap(L"\u00d7\0") };
	bitmap::BitMap<bitmap::Red> right{ font::Font::fnt.GetBitMap(L">") };

	std::vector<bitmap::BitMap<bitmap::Red>> result;

	for (auto& vector : scroll) {
		for (auto& elem : vector) {
			;
		}
	}
	return Texture{};
}