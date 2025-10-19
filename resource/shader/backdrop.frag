#version 330 core

/*
uniform bool focus;
uniform vec4 border;
*/
uniform sampler2D text;

in vec2 coord;

out vec4 color;

void main()
{
	//color = vec4(1., 0., 0., 1.);

	color = texture(text, coord);

	/*
	if (gl_FragCoord.x > border.x && gl_FragCoord.y > border.y &&
		gl_FragCoord.x < border.z && gl_FragCoord.y < border.w)
		color = texture(text, coord);
	if (focus) {
		color = vec4(1. - color.x, color.yzw );
	}
	*/
}