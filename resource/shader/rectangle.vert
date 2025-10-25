#version 420 core
layout (location = 0) in vec4 in_border;
layout (location = 1) in vec4 in_rectangle;
layout (location = 2) in vec4 in_region_0;

out vec4 vert_border;
out vec4 vert_rectangle;
out vec4 vert_region_0;

void main()
{
	vert_border = in_border;
	vert_rectangle = in_rectangle;
	vert_region_0 = in_region_0;
}
