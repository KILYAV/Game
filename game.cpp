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
	std::cout
		<< graphic::shape::Backdrop{ glm::vec2{} }.Type() << "\n";
}