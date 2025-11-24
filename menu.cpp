#include "window.h"
#include "font.h"

namespace window::menu {
	std::vector<graphic::texture::Texture> GetSizeScreen() {
		int count;
		auto modes{ glfwGetVideoModes(glfwGetPrimaryMonitor(), &count) };
		const std::vector<std::wstring> wstr_width{ std::invoke([&]() {
			std::vector<std::wstring> result;
			for (int index = 0; index < count; ++index) {
				result.push_back(std::to_wstring(modes[index].width));
			}
			return result;
			}) };
		const std::vector<std::wstring> wstr_height{ std::invoke([&]() {
			std::vector<std::wstring> result;
			for (int index = 0; index < count; ++index) {
				result.push_back(std::to_wstring(modes[index].height));
			}
			return result;
			}) };
		const glm::ivec2 size_width{ font::MaxSize(wstr_width) };
		const glm::ivec2 size_height{ font::MaxSize(wstr_height) };
		const int height = size_width.y > size_height.y ? size_width.y : size_height.y;
		const std::vector<font::Red> bitmap_width{
			font::GetVecBitmap(wstr_width, { size_width.x, height}, font::Center) };
		const std::vector<font::Red> bitmap_height{
			font::GetVecBitmap(wstr_height, { size_height.x, height}, font::Center) };
		const font::Red left{ font::GetBitmap(L"<") };
		const font::Red right{ font::GetBitmap(L">") };
		const font::Red center{ font::GetBitmap(L"\u00d7\0") };

		std::vector<graphic::texture::Texture> result;
		for (unsigned index = 0; index < count; ++index) {
			result.emplace_back(left + bitmap_width[index] + center + bitmap_height[index] + right);
		}
		return result;
	}
	void Options() {
		static Menu option{ std::invoke([]() {
			Menu menu;
			window::button::Button* button;
			window::button::Scroll* scroll;

			scroll = new window::button::Scroll(button::label::size_screen, UM_MENU_SETTING, UM_BUTTON_SIZESCREEN);
			*scroll = GetSizeScreen();
			scroll->SetTexture();
			menu.AddObject(scroll);

			button = new window::button::Button(button::label::exit, UM_MENU_SETTING, UM_BUTTON_EXIT);
			button->SetTexture(font::GetBitmap(button::label::exit));
			menu.AddObject(button);

			menu.SetSize(menu.DefaultSize());
			return menu;
			}) };
		int msg;
		do {
			msg = frame::Run(option);
			switch (msg) {
			case(UM_BUTTON_SIZESCREEN): {
				frame::frm.UpMode();
				static_cast<window::button::Scroll*>(option[button::label::size_screen])->UpTexture();
			}; break;
			}
		} while (UM_BUTTON_EXIT != msg);
	}
	void ModuleDesign() {
		static Menu design{ std::invoke([]() {
			Menu menu;
			window::button::Button* button;

			button = new window::button::Button(button::label::exit, UM_MENU_MODULE_DESIGN, UM_BUTTON_EXIT);
			button->SetTexture(font::GetBitmap(button::label::exit));
			menu.AddObject(button);

			menu.SetSize(frame::Size().integer);
			return menu;
			}) };
		
		if (auto size{ design.Rectangle::GetSize() }; frame::Size().integer.x != size.x
			|| frame::Size().integer.y != size.y)
			design.SetSize(frame::Size().integer);

		int msg;
		do {
			msg = frame::Run(design);
			switch (msg) {
			case(UM_BUTTON_SIZESCREEN): break;
			}
		} while (UM_BUTTON_EXIT != msg);
	}
	void Main() {
		static window::menu::Menu main{ std::invoke([]() {
			window::menu::Menu menu;
			window::button::Button* button;

			button = new window::button::Button(button::label::options, UM_MENU_MAIN, UM_BUTTON_SETTING);
			button->SetTexture(font::GetBitmap(button::label::options));
			menu.AddObject(button);

			button = new window::button::Button(button::label::module_design, UM_MENU_MAIN, UM_BUTTON_MODULE_DESIGN);
			button->SetTexture(font::GetBitmap(button::label::module_design));
			menu.AddObject(button);

			button = new window::button::Button(button::label::exit, UM_MENU_MAIN, UM_BUTTON_EXIT);
			button->SetTexture(font::GetBitmap(button::label::exit));
			menu.AddObject(button);

			menu.SetSize(menu.DefaultSize());
			return menu;
		}) };

		int msg{};
		do {
			msg = frame::Run(main);
			switch (msg) {
			case (UM_BUTTON_SETTING): Options(); break;
			case(UM_BUTTON_MODULE_DESIGN): ModuleDesign(); break;
			}
		} while (UM_BUTTON_EXIT != msg);
	}
}