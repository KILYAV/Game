#version 420 core
#define INVERT 0x01
#define BORDER 0x02
#define EQ(status,member) (member == (status & member))

layout (std140) uniform Button {
	int button_status;
};

uniform int rectangle_status;
uniform sampler2D texture_0;

in vec4 geom_border;
in vec2 geom_texture_0;

void Texture() {
	gl_FragColor = texture(texture_0, geom_texture_0);
}
void Invert(const int status) {
	if (EQ(status,INVERT))
		gl_FragColor = vec4(1. - gl_FragColor.r, gl_FragColor.gba);
}
void Border(const int status) {
	if (EQ(status,BORDER)) {
		if (gl_FragCoord.x > (geom_border.w - 0.) && gl_FragCoord.x < (geom_border.w + 1.))
			gl_FragColor = vec4(1 - gl_FragColor.x, gl_FragColor.yzw);
		if (gl_FragCoord.x > (geom_border.y - 1.) && gl_FragCoord.x < (geom_border.y + 0.))
			gl_FragColor = vec4(1 - gl_FragColor.x, gl_FragColor.yzw);
		if (gl_FragCoord.y > (geom_border.x - 1.) && gl_FragCoord.y < (geom_border.x + 0.))
			gl_FragColor = vec4(1 - gl_FragColor.x, gl_FragColor.yzw);
		if (gl_FragCoord.y > (geom_border.z - 0.) && gl_FragCoord.y < (geom_border.z + 1.))
			gl_FragColor = vec4(1 - gl_FragColor.x, gl_FragColor.yzw);
	}
}

void main()
{
	int status  = rectangle_status | button_status;

	Texture();
	Invert(status);
	Border(status);
}

