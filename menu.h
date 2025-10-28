#pragma once
#include <variant>
#include <optional>
#include "button.h"

namespace menu {
	auto& stt = setting::Setting::stt;
	template<template<class> class Layout, class... Button>
	struct Menu :
		Layout<Button...>,
		graphic::object::Rectangle<>,
		graphic::list::List<Button...>
	{
		using Rectangle = graphic::object::Rectangle<>;
		using List = graphic::list::List<Button...>;
		Menu(const glm::ivec2 pos = { 0, 0 }) :
			Menu{ pos, Layout<Button...>::UnitSize() }
		{}
	private:
		Menu(const glm::ivec2 pos, const glm::ivec2 size);
	public:
		static bool CallBack(void* instant, const glm::ivec2 pos,
			std::optional<std::tuple<int, int, int>> mouse) {
			return static_cast<Menu*>(instant)->ThisCallBack(pos, mouse);
		}
		static void CallPaint(void* instant) {
			static_cast<Menu*>(instant)->Paint();
		}
	protected:
		bool ThisCallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void Paint() {
			Rectangle::Paint();
			List::Paint();
		}
	};
	template<template<class> class Layout, class... Button>
	Menu<Layout,Button...>::Menu(const glm::ivec2 pos, const glm::ivec2 size) :
		Rectangle{ pos, glm::ivec2{
			size.x + size.y * stt.scale.menu,
			size.y * sizeof...(Button) * stt.scale.menu
		} },
		List{ static_cast<Layout<Button...>*>(this), Layout<Button...>::UnitPos(size, stt.scale.menu), size }
	{
		Rectangle::Border(true);
	}

	template<template<class> class Layout, class... Button>
	bool Menu<Layout, Button...>::ThisCallBack(const glm::ivec2 pos,
		std::optional<std::tuple<int, int, int>> mouse) {
		if (Rectangle::Region(pos)) {
			if (List::CallBack(pos, mouse))
				Rectangle::Focus(false);
			else
				Rectangle::Focus(true);
		}
		else
			return Rectangle::Focus(false);

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

	using Main = Menu<graphic::layout::Vertical, button::Nothing, button::Exit>;
}