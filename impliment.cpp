#include "setting.h"
#include "frame.h"
#include "font.h"
#include "graphics.h"
#include "object.h"
#include "resource.h"

setting::Setting setting::Setting::stt;
frame::Frame frame::Frame::frm;
font::Font font::Font::fnt;

const graphics::Shader graphics::Window::shader{ graphics::Shader::LoadShader(
	graphics::shader::label::button, IDR_SHADER_BUTTON_VERT, IDR_SHADER_BUTTON_FRAG) };

const std::map<const char*, unsigned> graphics::uniforms::Window::uniforms{
	graphics::uniforms::Window::GetUniforms(graphics::Window::shader.shader) };

object::Menu mn{ object::Menu::Init({ frm.size.integer.x >> 1, frm.size.integer.y >> 1 },
	object::Menu::Labels(object::button::Exit::label)) };
