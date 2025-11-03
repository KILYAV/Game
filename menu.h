#pragma once
#include <variant>
#include <optional>
#include "button.h"
#include "layout.h"
#include "function.h"

namespace menu {
	template<bool (*call_message)(int, int), template<class> class Layout, class... Unit>
	struct Menu :
		Layout<Unit...>,
		graphic::object::Rectangle<>,
		graphic::list::List<Unit...>
	{
		using Rectangle = graphic::object::Rectangle<>;
		using List = graphic::list::List<Unit...>;
		Menu(const glm::ivec2 pos = { 0, 0 }, const glm::ivec2 size = Size());
	public:
		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void Paint() {
			Rectangle::Paint();
			List::Paint();
		}
		bool CallMessage(int lParam, int wParam) {
			return (*call_message)(lParam, wParam);
		}
		static glm::ivec2 Step() {
			return graphic::layout::MaxSize<Unit...>();
		}
		static glm::ivec2 Size() {
			return Layout<Unit...>::Size(Step());
		}
	};
	template<bool (*call_messang)(int, int), template<class> class Layout, class... Unit>
	Menu<call_messang, Layout, Unit...>::Menu(const glm::ivec2 pos, const glm::ivec2 size) :
		Rectangle{ pos, size },
		List{ static_cast<Layout<Unit...>*>(this), pos, Step(), graphic::layout::MaxSize<Unit...>() }
	{
		Rectangle::Border(true);
	}
	template<bool (*call_messang)(int, int), template<class> class Layout, class... Unit>
	void Menu<call_messang, Layout, Unit...>::CallBack(const glm::ivec2 pos,
		std::optional<std::tuple<int, int, int>> mouse) {
		List::CallBack(pos, mouse);
	}

	using Main = Menu<function::main::Message, graphic::layout::Vertical,
		button::main::Setting, button::main::Exit>;
	using CancelApply = Menu<function::cancel_apply::CancelApply, graphic::layout::Horison,
		button::cancel_apply::Cancel, button::cancel_apply::Apply>;
	using Graphic = Menu<function::setting::graphic::Graphic, graphic::layout::Vertical,
		button::setting::FullScreen>;
	using Setting = Menu<function::setting::Setting, graphic::layout::Vertical,
		Graphic, CancelApply>;
}