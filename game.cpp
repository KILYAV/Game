#define LEON_OPENGL_IMPLEMENTATION
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "menu.h"

int main()
{
	menu::Main menu{};
	window::Window::frm << menu;
}