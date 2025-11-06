#pragma once
#include <variant>
#include <optional>
#include "graphic.h"
#include "layout.h"
#include "function.h"

namespace object {
	struct Object {
		virtual void Paint() const = 0;
		virtual void ReSize(const glm::ivec2 pos, const glm::ivec2 size) = 0;
		virtual glm::ivec2 NativeSize() = 0;
	};
	namespace button {
		struct Button :
			Object,
			graphic::object::Rectangle
		{
			const int wParam, lParam;

			using Rectangle = graphic::object::Rectangle;

			Button(const int i_wParam, const int i_lParam) :
				wParam{ i_wParam },
				lParam{ i_lParam }
			{
				Rectangle::Border(true);
			}
			void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
			glm::ivec2 NativeSize();
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
		/*
		namespace cancel_apply {
			using Apply = Button<UM_MENU_CANCEL_APPLY, UM_BUTTON_CANCEL,
				graphic::texture::str::Texture<label::apply>>;
			using Cancel = Button<UM_MENU_CANCEL_APPLY, UM_BUTTON_CANCEL,
				graphic::texture::str::Texture<label::cancel>>;
		}
		namespace setting {
			using SizeScreen = Button<UM_MENU_SETTING, UM_BUTTON_SIZESCREEN,
				graphic::texture::Texture>;
			using FullScreen = Button<UM_MENU_SETTING, UM_BUTTON_FULLSCREEN,
				graphic::texture::str::Texture<label::full_screen>>;
		}
		*/
	}
	namespace menu {
		template<class Layout>
		struct Menu {
		private:
			graphic::object::Rectangle rectangle;
			std::vector<std::unique_ptr<object::Object>> objects;
		public:
			Menu() {
				rectangle.Border(true);
			}
			//Menu(const glm::ivec2 pos, const glm::ivec2 size);
			template<class Object>
			void AddObject(Object obj) {
				objects.push_back(std::unique_ptr<object::Object>{ static_cast<object::Object*>(obj) });
			}
			void ReSize();
		public:
			void CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse);
			void Paint() {
				rectangle.Paint();
				for (auto& object : objects) {
					object->Paint();
				}
			}
			bool CallMessage(int lParam, int wParam) {
				return false;// (*call_message)(lParam, wParam);
			}
		};
		template<class Layout>
		void Menu<Layout>::ReSize() {
			glm::ivec2 size{ 0,0 };
			for (auto& object : objects) {
				auto native = object->NativeSize();
				size.x = size.x > native.x ? size.x : native.x;
				size.y = size.y > native.y ? size.y : native.y;
			}
			
			rectangle.SetPos({0,0});
			rectangle.SetSize(Layout::Size(size, objects.size()));
			rectangle.SetRegion();
			rectangle.LoadVertex(rectangle.GetBorder(), rectangle.GetRectangle());
			
			for (int index = 0, count = objects.size(); index < count; ++index) {
				objects[index]->ReSize(Layout::Order(rectangle.GetPos(), size, count, index), size);
			}
		}
		template<class Layout>
		void Menu<Layout>::CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
			//List::CallBack(pos, mouse);
		}

		/*
		using Setting = Menu<function::setting::Setting, graphic::layout::Vertical,
			//button::setting::SizeScreen,
			button::setting::FullScreen,
			button::Exit<UM_MENU_MAIN, UM_BUTTON_EXIT>>;
			*/
	}
}