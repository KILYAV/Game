#pragma once
#include <variant>
#include <optional>
#include "frame.h"
#include "graphic.h"

namespace function {
	void Nothing() {
		;
	}
	void Exit() {
		PostQuitMessage(0);
	}
}
namespace button {
	template<void (*Function)(), class Texture>
	struct Button :
		Texture,
		graphic::object::Rectangle<Texture>
	{
		using Rectangle = graphic::object::Rectangle<Texture>;
		Button(const std::pair<glm::ivec2, glm::ivec2> input);
		bool CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
			return Rectangle::CallBack(pos, mouse);
		}
		static bool CallBack(void* instant, const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			return static_cast<Button<Function, Texture>*>(instant)->CallBack(pos, mouse);
		}
		static void CallPaint(void* instant);
	};
	template<void (*Function)(), class Texture>
	Button<Function, Texture>::Button(const std::pair<glm::ivec2, glm::ivec2> input) :
		Texture{},
		Rectangle{ std::pair{ input, std::array<glm::ivec2, 2>{ glm::ivec2{}, Texture::texture.size } } }
	{
		Rectangle::Border(true);
	}
	template<void (*Function)(), class Texture>
	void Button<Function, Texture>::CallPaint(void* instant) {
		auto This{ static_cast<Button<Function, Texture>*>(instant) };
		This->Rectangle::CallPaint(static_cast<Rectangle*>(This));
	}

	namespace label {
		static constexpr wchar_t exit[] = L"EXIT";
		static constexpr wchar_t nothing[] = L"NOTHING";
	}
	using Exit = Button<function::Exit, graphic::texture::str::Texture<label::exit>>;
	using Nothing = Button<function::Nothing, graphic::texture::str::Texture<label::nothing>>;
}