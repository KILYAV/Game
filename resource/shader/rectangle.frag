#version 460 core
#define FOCUS  0x01
#define INVERT 0x02
#define BORDER 0x04
#define EQ(status,member) (member == (status & member))

layout (location = 0) in vec4 in_border;
layout (location = 1) in vec2 in_texture;

layout(std140, binding = 0) uniform frame {
	vec2 size;
	vec2 pixel;
	ivec2 integer;
	ivec2 center;
};

uniform int rectangle_status;

uniform sampler2D texture_0;

void main()
{
	int status = rectangle_status;

	gl_FragColor = vec4(0.,0.,0.,0.);

	if (EQ(status,BORDER)) {
		if (gl_FragCoord.x > in_border.x || gl_FragCoord.x < in_border.z)
			gl_FragColor.r = 1 - gl_FragColor.r;
		if (gl_FragCoord.y > in_border.y || gl_FragCoord.y < in_border.w)
			gl_FragColor.r = 1 - gl_FragColor.r;
	}

	gl_FragColor += texture(texture_0, in_texture);

	if (EQ(status,FOCUS) && EQ(status,INVERT))
		gl_FragColor.r = 1. - gl_FragColor.r;
}

