#version 330 core
uniform int status;
uniform sampler2D texture_0;

in vec2 coord_0;

void main()
{
	gl_FragColor = texture(texture_0, coord_0);
	if (1 == (status & 1))
		gl_FragColor = vec4(1. - gl_FragColor.r, gl_FragColor.gba);
	/*
	if (gl_FragCoord.x > border.x && gl_FragCoord.y > border.y &&
		gl_FragCoord.x < border.z && gl_FragCoord.y < border.w)
		color = texture(text, coord);
	if (focus) {
		color = vec4(1. - color.x, color.yzw );
	}
	*/
}
