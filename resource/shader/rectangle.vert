#version 330 core
layout (location = 0) in vec4 in_rectangle;
layout (location = 1) in vec4 in_region_0;

out vec4 rectangle;
out vec4 region_0;

uniform int status;

void main()
{
	rectangle = in_rectangle;
	region_0 = in_region_0;
}
