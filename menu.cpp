#include "window.h"
#include "menu.h"

namespace menu {
	/*
	void Setting() {
		static window::menu::Menu setting{ std::invoke([]() {
			window::menu::Menu menu;
			window::button::Button* button;

			button = new window::button::Button(UM_MENU_SETTING, UM_BUTTON_SIZESCREEN);
			{
				graphic::texture::Scroll scroll;
				std::vector<std::wstring> width, height;
				int count;
				auto modes{ glfwGetVideoModes(glfwGetPrimaryMonitor(), &count) };
				for (int index = 0; index < count; ++index) {
					auto& mode{ modes[index] };
					width.push_back(std::to_wstring(mode.width));
					height.push_back(std::to_wstring(mode.height));
				}
				glm::ivec2 size{ font::MaxSize(width) };

				scroll.AddVector(std::move(width)).AddVector(std::move(height)).Release();

				button->AddTexture(graphic::texture::Texture{}.AddTexture(L"<"));
				button->AddTexture(graphic::texture::Texture{}.AddTexture(L"\u00d7\0"));
				button->AddTexture(graphic::texture::Texture{}.AddTexture(L">"));
			}
			menu.AddObject(button);

			button = new window::button::Button(UM_MENU_SETTING, UM_BUTTON_EXIT);
			button->AddTexture(window::button::label::exit);
			menu.AddObject(button);

			return menu;
			}) };
		int msg;
		do {
			msg = frame::Frame::frm.Run(setting);
			switch (msg) {
			case(UM_BUTTON_FULLSCREEN): {

			}; break;
			}
		} while (UM_BUTTON_EXIT != msg);
	}
	*/
	void Main() {
		static window::menu::Menu main{ std::invoke([]() {
			window::menu::Menu menu;
			window::button::Button* button;

			button = new window::button::Button(UM_MENU_MAIN, UM_BUTTON_SETTING);
			button->AddTexture(window::button::label::setting);
			menu.AddObject(button);

			button = new window::button::Button(UM_MENU_MAIN, UM_BUTTON_EXIT);
			button->AddTexture(window::button::label::exit);
			menu.AddObject(button);

			return menu;
		}) };

		int msg{};
		do {
			msg = frame::Frame::frm.Run(main);
			switch (msg) {
			case (UM_BUTTON_SETTING): /*Setting();*/ break;
			}
		} while (UM_BUTTON_EXIT != msg);
	}
}