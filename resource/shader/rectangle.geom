#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 region_0[];
out vec2 region_2;

vec2[4] Unpuck(const vec4 region) {
	vec2[4] result;
	result[0] = vec2(region.w, region.x);
	result[1] = vec2(region.y, region.x);
	result[2] = vec2(region.w, region.z);
	result[3] = vec2(region.y, region.z);
	return result;
}

void main()
{
	vec2[4] position = Unpuck(gl_in[0].gl_Position);
	vec2[4] coord = Unpuck(region_0[0]);

	for (int index = 0; index < 4; ++index) {
		gl_Position = vec4(position[index], 0., 1.);
		region_2 = coord[index];
		EmitVertex();
	}
	EndPrimitive();
}