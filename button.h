#pragma once
#include <variant>
#include <optional>
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "graphic.h"
#include "function.h"

namespace button {
	template<int wParam, int lParam, class Texture>
	struct Button :
		Texture,
		graphic::object::Rectangle<Texture>
	{
		using Rectangle = graphic::object::Rectangle<Texture>;

		Button(const glm::ivec2 pos = { 0, 0 }, const glm::ivec2 size = Size());
		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		static glm::ivec2 Size();
	};
	template<int wParam, int lParam, class Texture>
	Button<wParam, lParam, Texture>::Button(const glm::ivec2 pos, const glm::ivec2 size) :
		Texture{},
		Rectangle{ pos, std::invoke([&]() {
			int Y = size.y / 1.5 * 1.25;
			int X = size.y - Y;
			return glm::ivec2{ size.x - X, Y };
			}), std::pair{glm::ivec2{}, Texture::texture.size} }
	{
		Rectangle::Border(true);
	}
	template<int wParam, int lParam, class Texture>
	void Button<wParam, lParam, Texture>::CallBack(
		const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		if (Rectangle::Region(pos)) {
			Rectangle::Focus(true);
			if (mouse) {
				auto tuple{ mouse.value() };
				int button{ std::get<0>(tuple) };
				int action{ std::get<1>(tuple) };
				int mods{ std::get<2>(tuple) };
				if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action) {
					PostMessage(NULL, WM_USER, wParam, lParam);
				}
			}
		}
		else {
			Rectangle::Focus(false);
		}
	};
	template<int wParam, int lParam, class Texture>
	glm::ivec2 Button<wParam, lParam, Texture>::Size() {
		glm::ivec2 size { Texture::texture.size };
		return { size.x + size.y, size.y * 1.5 };
	}

	namespace label {
		static constexpr wchar_t exit[] = L"EXIT";
		static constexpr wchar_t apply[] = L"APPLY";
		static constexpr wchar_t cancel[] = L"CANCEL";
		static constexpr wchar_t setting[] = L"SETTING";
		static constexpr wchar_t full_screen[] = L"FULLSCREEN";
	}
	namespace main {
		using Exit = Button<UM_MENU_MAIN, UM_BUTTON_EXIT,
			graphic::texture::str::Texture<label::exit>>;
		using Setting = Button<UM_MENU_MAIN, UM_BUTTON_SETTING,
			graphic::texture::str::Texture<label::setting>>;
	}
	namespace cancel_apply {
		using Apply = Button<UM_MENU_CANCEL_APPLY, UM_BUTTON_CANCEL,
			graphic::texture::str::Texture<label::apply>>;
		using Cancel = Button<UM_MENU_CANCEL_APPLY, UM_BUTTON_CANCEL,
			graphic::texture::str::Texture<label::cancel>>;
	}
	namespace setting {
		using FullScreen = Button<UM_MENU_SETTING, UM_BUTTON_FULLSCREEN,
			graphic::texture::str::Texture<label::full_screen>>;
	}
}