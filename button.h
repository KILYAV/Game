#pragma once
#include <variant>
#include <optional>
#include "frame.h"
#include "graphic.h"

namespace function {
	void Nothing(int action) {
		std::cout << action << "\n";
	}
	void Exit(int) {
		PostQuitMessage(0);
	}
}
namespace button {
	auto& stt = setting::Setting::stt;
	template<void (*Function)(int), class Texture>
	struct Button :
		Texture,
		graphic::object::Rectangle<Texture>
	{
		using Rectangle = graphic::object::Rectangle<Texture>;
		using Rectangle::Paint;

		Button(const std::pair<glm::ivec2, glm::ivec2> input);
		bool CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
			if (Rectangle::Region(pos)) {
				if (mouse) {
					auto tuple{ mouse.value() };
					int button{ std::get<0>(tuple) };
					int action{ std::get<1>(tuple) };
					int mods{ std::get<2>(tuple) };
					if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action)
						Function(action);
				}
				return Rectangle::Focus(true);
			}
			else {
				return Rectangle::Focus(false);
			}
		}
	};
	template<void (*Function)(int), class Texture>
	Button<Function, Texture>::Button(const std::pair<glm::ivec2, glm::ivec2> input) :
		Texture{},
		Rectangle{ std::pair{ std::invoke([&](const glm::ivec2 pos, const glm::ivec2 size) {
			int X = size.x + size.y * stt.scale.button;
			int Y = size.y * stt.scale.button;
			return std::pair{ pos, glm::ivec2{ X, Y } };
			}, input.first, input.second),
		std::array<glm::ivec2, 2>{ glm::ivec2{}, Texture::texture.size } } }
	{
		Rectangle::Border(true);
	}

	namespace label {
		static constexpr wchar_t exit[] = L"EXIT";
		static constexpr wchar_t nothing[] = L"NOTHING";
	}
	using Exit = Button<function::Exit, graphic::texture::str::Texture<label::exit>>;
	using Nothing = Button<function::Nothing, graphic::texture::str::Texture<label::nothing>>;
}