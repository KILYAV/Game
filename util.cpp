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
namespace glm {
	vec4 Rectangle(const vec2 center, const vec2 size) {
		vec4 value;
		value.x = center.x - size.x / 2;
		value.y = center.y - size.y / 2;
		value.z = value.x + size.x;
		value.w = value.y + size.y;
		return value;
	}
}