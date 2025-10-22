#version 330 core
layout (location = 0) in vec4 rectangle;
layout (location = 1) in vec4 texture_0;
layout (location = 2) in vec4 texture_1;

out vec4 region_0;

void main()
{
	gl_Position = rectangle;
	region_0 = texture_0;
}
