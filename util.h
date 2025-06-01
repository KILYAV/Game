#pragma once
#include <Windows.h>
#include "bitmap.h"
#include "stb_image.h"

namespace util {
	template<typename Pixel>
	bitmap::BitMap<Pixel> LoadTexture(const int ID) {
		auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), reinterpret_cast<LPCSTR>(L"JPG"));
		auto source{ LockResource(LoadResource(NULL, resource)) };
		auto size{ SizeofResource(NULL, resource) };
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load_from_memory(static_cast<const unsigned char*>(source), size, &width, &height, &channels, 0);
		return bitmap::BitMap<Pixel>{ static_cast<Pixel*>(static_cast<void*>(data)), { width,height } };
	}
	//bitmap::BitMap LoadTexture(const std::string& path);
}
