#version 460 core
layout (location = 0) in ivec4 in_rectangle;
layout (location = 1) in ivec4 in_texture;

layout (location = 0) out vec4 out_border;
layout (location = 1) out vec4 out_texture;

layout(std140, binding = 0) uniform frame {
	vec4 floating;
	vec4 pixel;
	ivec4 integer;
	ivec4 center;
};

void main()
{
	vec2 size;
	size.x = in_rectangle.x - in_rectangle.z;
	size.y = in_rectangle.y - in_rectangle.w;

	vec2 pos;
	pos.x = in_texture.x / size.x;
	pos.y = in_texture.y / size.y;

	size.x = size.x / in_texture.z * .5;
	size.y = size.y / in_texture.w * .5;

	out_texture.x = .5 + pos.x + size.x;
	out_texture.y = .5 + pos.y + size.y;
	out_texture.z = .5 + pos.x - size.x;
	out_texture.w = .5 + pos.y - size.y;

	out_border.x = in_rectangle.x + center.x - 1;
	out_border.y = in_rectangle.y + center.y - 1;
	out_border.z = in_rectangle.z + center.x + 1;
	out_border.w = in_rectangle.w + center.y + 1;

	gl_Position.x = in_rectangle.x * pixel.x * 2;
	gl_Position.y = in_rectangle.y * pixel.y * 2;
	gl_Position.z = in_rectangle.z * pixel.x * 2;
	gl_Position.w = in_rectangle.w * pixel.y * 2;
}
