#include "window.h"

namespace window::button {
	void Button::CallBack(
		const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		if (SetFocus(Border(pos))) {
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
		auto native{ Rectangle::Texture::GetSize() };
		Rectangle::SetCenter(pos);
		Rectangle::SetSize(std::invoke([&]() {
			return glm::ivec2{ size.x - native.y * .5, native.y * 1.25 };
			}));
		Rectangle::LoadVertex(Rectangle::GetBorder(), {0,0,native.x,native.y});
	}
	void Scroll::operator= (const std::vector<graphic::texture::Texture>& vector) {
		textures = vector;
	}
	void Scroll::SetTexture() {
		if (textures.size()) {
			index = 0;
			Button::SetTexture(textures[index]);
		}
	}
	void Scroll::UpTexture() {
		if (textures.size()) {
			index = (index + 1) == textures.size() ? 0 : index + 1;
			Button::SetTexture(textures[index]);
		}
	}
}
namespace window::menu {
	void Menu::Paint() {
		Rectangle::Paint();
		for (auto& object : objects) {
			object->Paint();
		}
	}
	void Menu::SetSize(const glm::ivec2 size) {
		Rectangle::SetCenter({ 0,0 });
		Rectangle::SetSize(layout.GetSize(size, objects.size()));
		LoadVertex(GetBorder());

		for (int index = 0, count = objects.size(); index < count; ++index) {
			objects[index]->ReSize(layout.GetPos(GetCenter(), size, count, index), size);
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
	void Menu::CallBack(const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		for (auto& object : objects) {
			object->CallBack(pos, mouse);
		}
	}
	Window* Menu::operator[] (const wchar_t* label) {
		Window* result = nullptr;
		for (auto& object : objects) {
			if (label == object->Label())
				result = object.get();
		}
		return result;
	}
}