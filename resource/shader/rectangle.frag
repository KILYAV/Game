#version 330 core

uniform vec4 background_color;
uniform sampler2D texture_0;

in vec2 region_2;

void main()
{
	gl_FragColor = /*vec4(1., 1., 1., 1.);//*/texture(texture_0, region_2);

	/*
	if (gl_FragCoord.x > border.x && gl_FragCoord.y > border.y &&
		gl_FragCoord.x < border.z && gl_FragCoord.y < border.w)
		color = texture(text, coord);
	if (focus) {
		color = vec4(1. - color.x, color.yzw );
	}
	*/
}