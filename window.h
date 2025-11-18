#pragma once
#include <variant>
#include <optional>
#include "graphic.h"
#include "function.h"

namespace window {
	struct Window {
		virtual void Paint() const = 0;
		virtual void ReSize(const glm::ivec2 pos, const glm::ivec2 size) = 0;
		virtual void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) = 0;
		virtual glm::ivec2 DefaultSize() = 0;
	};
}
namespace window::button {
	struct Button :
		Window,
		graphic::object::Rectangle
	{
		const int wParam, lParam;

		using Rectangle = graphic::object::Rectangle;

		Button(const int i_wParam, const int i_lParam) :
			wParam{ i_wParam },
			lParam{ i_lParam }
		{
			Rectangle::Shader::SetBorder(true);
		}
		glm::ivec2 DefaultSize();
		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void ReSize(const glm::ivec2 pos, const glm::ivec2 size);
		void Paint() const {
			Rectangle::Paint();
		}
	};
	namespace label {
		static constexpr wchar_t exit[] = L"EXIT";
		static constexpr wchar_t apply[] = L"APPLY";
		static constexpr wchar_t cancel[] = L"CANCEL";
		static constexpr wchar_t setting[] = L"SETTING";
		static constexpr wchar_t size_screen[] = L"Size screen";
		static constexpr wchar_t full_screen[] = L"FULLSCREEN";
	}
}
namespace window::menu {
	struct Menu {
	private:
		graphic::layout::Layout layout;
		graphic::object::Rectangle rectangle;
		std::vector<std::unique_ptr<window::Window>> objects;
	public:
		Menu() {
			rectangle.Shader::SetBorder(true);
		}
		template<class Object>
		void AddObject(Object obj) {
			objects.push_back(std::unique_ptr<window::Window>{ static_cast<window::Window*>(obj) });
			ReSize();
		}
		glm::ivec2 DefaultSize() const;
		void ReSize();
	public:
		void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
		void Paint();
		bool CallMessage(int lParam, int wParam) {
			return false;// (*call_message)(lParam, wParam);
		}
	};
}