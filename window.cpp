#include "window.h"

namespace window::button {
	void Button::CallBack(
		const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		if (Rectangle::SetFocus(Rectangle::Border(pos))) {
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
	};
	glm::ivec2 Button::DefaultSize() {
		glm::ivec2 size{ Rectangle::Texture::GetSize() };
		return { size.x + size.y, size.y * 1.5 };
	}
	void Button::ReSize(const glm::ivec2 pos, const glm::ivec2 size) {
		Rectangle::Array::SetCenter(pos);
		Rectangle::Texture::SetCenter();
		Rectangle::SetSize(std::invoke([&]() {
			auto native{ Rectangle::Texture::GetSize() };
			return glm::ivec2{ size.x - native.y * .5, native.y * 1.25 };
			}));
		Rectangle::LoadVertex(Rectangle::GetBorder(), Rectangle::Texture::GetTexture());
	}
}
namespace window::menu {
	void Menu::Paint() {
		rectangle.Paint();
		for (auto& object : objects) {
			object->Paint();
		}
	}
	glm::ivec2 Menu::DefaultSize() const {
		glm::ivec2 size{ 0,0 };
		for (auto& object : objects) {
			auto native = object->DefaultSize();
			size.x = size.x > native.x ? size.x : native.x;
			size.y = size.y > native.y ? size.y : native.y;
		}
		return size;
	}
	void Menu::ReSize() {
		glm::ivec2 size{ DefaultSize() };

		rectangle.Array::SetCenter({ 0,0 });
		rectangle.SetSize(layout.GetSize(size, objects.size()));
		rectangle.LoadVertex(rectangle.GetBorder());

		for (int index = 0, count = objects.size(); index < count; ++index) {
			objects[index]->ReSize(layout.GetPos(rectangle.GetCenter(), size, count, index), size);
		}
	}
	void Menu::CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		for (auto& object : objects) {
			object->CallBack(pos, mouse);
		}
	}
}