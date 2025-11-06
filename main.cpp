#define LEON_OPENGL_IMPLEMENTATION
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "menu.h"

#include "message.h"

/*
void Setting() {
	menu::Setting setting;
	int msg;
	do {
		msg = frame::Frame::frm.Run(setting);
		switch (msg) {
		case(UM_BUTTON_FULLSCREEN): {
			setting::Setting::stt.FullScreen(false == setting::Setting::stt.FullScreen());
			frame::Frame::frm.FullScreen();
			menu::Setting setting;
			frame::Frame::frm.Run(setting);
			}; break;
		}
	} while (UM_BUTTON_EXIT != msg);
}
*/
void Main() {
	object::menu::Menu<graphic::layout::Vertical> main;
	std::unique_ptr<object::button::Button> button;

	button = std::make_unique<object::button::Button>(UM_MENU_MAIN, UM_BUTTON_EXIT);
	button->AddTexture(object::button::label::exit);
	main.AddObject(button.release());
	
	button = std::make_unique<object::button::Button>(UM_MENU_MAIN, UM_BUTTON_SETTING);
	button->AddTexture(object::button::label::setting);
	main.AddObject(button.release());
	
	main.ReSize();

	int msg{};
	do {
		msg = frame::Frame::frm.Run(main);
		switch (msg) {
		case (UM_BUTTON_SETTING): /*Setting();*/ break;
		}
	} while (UM_BUTTON_EXIT != msg);
}

int main()
{
	Main();
}