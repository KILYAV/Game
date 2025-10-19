#include <iostream>
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "graphic.h"
//#include "object.h"

setting::Setting setting::Setting::stt;
frame::Frame frame::Frame::frm;
font::Font font::Font::fnt;

int main()
{
	graphic::object::button::Exit{ { frm.size.integer.x / 2, frm.size.integer.y / 2 } }.Draw();
	glfwSwapBuffers(frm.window);
	char ch;
	std::cin >> ch;
}