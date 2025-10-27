#pragma once
#include <variant>
#include <optional>
#include "button.h"

namespace menu {
	template<class... Button>
	struct Menu :
		graphic::object::Rectangle<>,
		graphic::list::List<graphic::list::layout::Vertical, Button...>
	{
		using Rectangle = graphic::object::Rectangle<>;
		using List = graphic::list::List<graphic::list::layout::Vertical, Button...>;
		Menu();
		static bool CallBack(void* instant, const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			return static_cast<Menu<Button...>*>(instant)->ThisCallBack(pos, mouse);
		}
		static void CallPaint(void* instant) {
			static_cast<Menu<Button...>*>(instant)->ThisCallPaint();
		}
	protected:
		bool ThisCallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void ThisCallPaint() {
			Rectangle::ThisCallPaint();
			List::ThisCallPaint();
		}
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
			}) }
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
			}) }
	{}
	template<class... Button>
	Menu<Button...>::Menu(const glm::ivec2 size,
		const std::pair<int, std::array<int, sizeof...(Button)>> pos, int index) :
		Rectangle{ std::invoke([&]() {
			int W{ static_cast<int>(size.x + size.y * (scale - 1)) };
			int H{ static_cast<int>(size.y * scale * pos.second.size()) };
			return std::pair{ glm::ivec2{ 0, 0 }, glm::ivec2{ W, H } };
			}) },
		List{ std::pair{ pos, size } }
	{
		Rectangle::Border(true);
	}
	template<class... Button>
	bool Menu<Button...>::ThisCallBack(const glm::ivec2 pos,
		std::optional<std::tuple<int, int, int>> mouse) {
		if (List::ThisCallBack(pos, mouse))
			return true;

		/*
		if (This->List::CallBack(This, pos, mouse)) {
			This->Rectangle::Focus(false);
			return true;
		}
		else if (This->Rectangle::CallBack(This, pos, mouse)) {
			return true;
		}
		else
			return false;
		*/
	}

	using Main = Menu<button::Nothing, button::Exit>;
}