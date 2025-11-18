#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (location = 0) in vec4 in_border[];
layout (location = 1) in vec4 in_texture[];

layout (location = 0) out vec4 out_border;
layout (location = 1) out vec2 out_texture;

layout(std140, binding = 0) uniform frame {
	vec2 size;
	vec2 pixel;
	ivec2 integer;
	ivec2 center;
};

void main()
{
	vec2[4] position;
	position[0].x = gl_in[0].gl_Position.z;
	position[0].y = gl_in[0].gl_Position.y;
	position[1].x = gl_in[0].gl_Position.x;
	position[1].y = gl_in[0].gl_Position.y;
	position[2].x = gl_in[0].gl_Position.z;
	position[2].y = gl_in[0].gl_Position.w;
	position[3].x = gl_in[0].gl_Position.x;
	position[3].y = gl_in[0].gl_Position.w;

	vec2[4] texture_coord;
	texture_coord[0].x = in_texture[0].z;
	texture_coord[0].y = in_texture[0].y;
	texture_coord[1].x = in_texture[0].x;
	texture_coord[1].y = in_texture[0].y;
	texture_coord[2].x = in_texture[0].z;
	texture_coord[2].y = in_texture[0].w;
	texture_coord[3].x = in_texture[0].x;
	texture_coord[3].y = in_texture[0].w;

	for (int index = 0; index < 4; ++index) {
		gl_Position = vec4(position[index], 0., 1.);
		out_border = in_border[0];
		out_texture = texture_coord[index];
		EmitVertex();
	}
	EndPrimitive();
}