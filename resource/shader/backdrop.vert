#version 330 core
layout (location = 0) in vec2 in_coord;
layout (location = 1) in vec2 in_pos;

out vec2 coord;

void main()
{
	gl_Position = vec4(in_pos,0,1);
	coord = in_coord;
}
