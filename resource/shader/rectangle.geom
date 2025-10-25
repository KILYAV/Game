#version 420 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 vert_border[];
in vec4 vert_rectangle[];
in vec4 vert_region_0[];

out vec4 geom_border;
out vec2 geom_texture_0;

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
	vec2[4] position = Unpuck(vert_rectangle[0]);
	vec2[4] texture_ = Unpuck(vert_region_0[0]);

	geom_border = vert_border[0];
	for (int index = 0; index < 4; ++index) {
		gl_Position = vec4(position[index], 0., 1.);
		geom_texture_0 = texture_[index];
		geom_border = vert_border[0];
		EmitVertex();
	}
	EndPrimitive();
}