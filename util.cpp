#include "util.h"
#define STB_IMAGE_IMPLEMENTATION

/*
domain::BitMap util::LoadTexture(const std::string& path) {
	static std::string parent_path{ "resources\\textures\\" };
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	std::string name{ parent_path + path };
	unsigned char* data = stbi_load(name.data(), &width, &height, &channels, 0);
	return domain::BitMap{ data, { width,height }, channels };
}
*/