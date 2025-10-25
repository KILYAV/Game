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
		using Rectangle::Draw;
		Button(const std::optional<glm::ivec2> pos = std::nullopt,
			const std::optional<glm::ivec2> size = std::nullopt);
		static void CallBack(void* instant,
			const std::variant<glm::ivec2, std::tuple<int,int,int>> variant);
	};
	template<void (*Function)(), class Texture>
	Button<Function, Texture>::Button(const std::optional<glm::ivec2> pos,
		const std::optional<glm::ivec2> size) :
		Texture{},
		Rectangle{
			std::pair<std::pair<glm::ivec2, glm::ivec2>, std::array<glm::ivec2, 2>> {
				std::pair<glm::ivec2, glm::ivec2>{
					pos ? pos.value() : glm::ivec2{},
					size ? size.value() : Texture::texture.size,
				},
				std::array<glm::ivec2, 2>{
					glm::ivec2{},
					Texture::texture.size
				}
			}
		}
	{
		Rectangle::Border(true);
	}
	template<void (*Function)(), class Texture>
	void Button<Function, Texture>::CallBack(void* instant,
		const std::variant<glm::ivec2, std::tuple<int, int, int>> variant) {
		auto This{ static_cast<Button<Function, Texture>*>(instant) };
		if (variant.index()) {
			Function();
		}
		else {
			glm::ivec2 pos{ std::get<0>(variant) };
			if (This->Rectangle::Region(pos))
				This->Rectangle::Invert(true);
			else
				This->Rectangle::Invert(false);
		}
	}

	namespace label {
		static constexpr wchar_t exit[] = L"EXIT";
		static constexpr wchar_t nothing[] = L"NOTHING";
	}
	using Exit = Button<function::Exit, graphic::texture::str::Texture<label::exit>>;
	using Nothing = Button<function::Nothing, graphic::texture::str::Texture<label::nothing>>;
}
namespace menu {
	template<class... Button>
	struct Menu :
		graphic::object::Rectangle<>,
		Button...
	{
		using Rectangle = graphic::object::Rectangle<>;
		using Rectangle::Draw;
		Menu();
		void Draw() const;
		static void CallBack(void* instant,
			const std::variant<glm::ivec2, std::tuple<int, int, int>> variant);
	private:
		static constexpr float scale = 1.25;
		Menu(const glm::ivec2 size);
		Menu(const glm::ivec2 size,
			const std::pair<int, std::array<int, sizeof...(Button)>> pos, int index = 0);
	};
	template<class... Button>
	Menu<Button...>::Menu() :
		Menu{ std::invoke([]() {
			glm::ivec2 size{ 0,0 };
			(std::invoke([&]() {
				size.x = size.x < Button::texture.size.x ? Button::texture.size.x : size.x;
				size.y = size.y < Button::texture.size.y ? Button::texture.size.y : size.y;
				}), ...);
			return glm::ivec2{ size.x * scale, size.y * scale };
			})}
	{}
	template<class... Button>
	Menu<Button...>::Menu(const glm::ivec2 size) :
		Menu{ size, std::invoke([&]() {
			std::array<int, sizeof...(Button)> pos;
			int step{ static_cast<int>(size.y * scale) };
			int H{ static_cast<int>(step * .5 * (sizeof...(Button) - 1)) };
			for (int index = 0; index < sizeof...(Button); ++index, H -= step) {
				pos[index] = H;
			}
			return std::pair{ 0, pos };
			})}
	{}
	template<class... Button>
	Menu<Button...>::Menu(const glm::ivec2 size,
		const std::pair<int, std::array<int, sizeof...(Button)>> pos, int index) :
		Rectangle{ std::invoke([&]() {
			int W{ static_cast<int>(size.x + size.y * (scale - 1)) };
			int H{ static_cast<int>(size.y * scale * pos.second.size()) };
			return std::pair{ glm::ivec2{ 0, 0 }, glm::ivec2{ W, H } };
			}) },
		Button{ std::invoke([&]() {
			auto& X{ pos.first };
			auto& Y{ pos.second };
			glm::ivec2 result;
			result.x = X;
			result.y = Y[index++];
			return result;
			}), size }...
	{
		Rectangle::Border(true);
	}
	template<class... Button>
	void Menu<Button...>::Draw() const {
		Rectangle::Draw();
		(std::invoke([&]() {
			Button::Draw();
			}), ...);
	}
	template<class... Button>
	void Menu<Button...>::CallBack(void* instant,
		const std::variant<glm::ivec2, std::tuple<int, int, int>> variant) {
		auto This{ static_cast<Menu<Button...>*>(instant) };
		if (variant.index()) {
			;
		}
		else {
			glm::ivec2 pos{ std::get<0>(variant) };
			if (This->Rectangle::Region(pos))
				This->Rectangle::Invert(true);
			else
				This->Rectangle::Invert(false);
		}
	}

	using Main = Menu<button::Nothing, button::Exit>;
}