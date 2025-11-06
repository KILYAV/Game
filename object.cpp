#include "menu.h"

namespace object {
	namespace button {
		void Button::CallBack(
			const glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
			if (Rectangle::Region(pos)) {
				Rectangle::Focus(true);
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
			else {
				Rectangle::Focus(false);
			}
		};
		glm::ivec2 Button::NativeSize() {
			glm::ivec2 size{ Rectangle::Texture::MaxSize() };
			return { size.x + size.y, size.y * 1.5 };
		}
		void Button::ReSize(const glm::ivec2 pos, const glm::ivec2 size) {
			Rectangle::SetPos(pos);
			Rectangle::SetSize(std::invoke([&]() {
				auto native { Rectangle::Texture::MaxSize() };
				return glm::ivec2{ size.x - native.y * .5, native.y * 1.25 };
				}));
			Rectangle::SetRegion();
			Rectangle::LoadVertex(Rectangle::GetBorder(), Rectangle::GetRectangle(),
				Rectangle::Rectangle::GetTexture(Rectangle::Texture::GetTexture()));
		}
	}
	namespace menu {

	}
}