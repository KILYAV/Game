#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 rectangle[];
in vec4 region_0[];

out vec2 coord_0;

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
	vec2[4] rectangle_ = Unpuck(rectangle[0]);
	vec2[4] texture_ = Unpuck(region_0[0]);

	for (int index = 0; index < 4; ++index) {
		gl_Position = vec4(rectangle_[index], 0., 1.);
		coord_0 = texture_[index];
		EmitVertex();
	}
	EndPrimitive();
}