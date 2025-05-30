#include <Windows.h>
#include <string>
#include "bitmap.h"
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "object.h"

using namespace object;
Button::Button(const wchar_t* label, const glm::ivec2 center, const glm::ivec2 size) :
	graphics::Window{},
	border{ center, size }
{
	LoadVertices(graphics::vertex::Array::GetRectangle(border.value));
	LoadElement({});
	LoadTexture(bitmap::BitMap<bitmap::Red>{ nullptr, size }
		.Fill({ 0 }).Insert(fnt.GetBitMap(label), { (size.x - fnt.Leingth(label)) >> 1 ,0 }));
	static_cast<graphics::uniforms::Flash&>(*this).value = false;
	static_cast<graphics::uniforms::Rectangle&>(*this) = border - 1;
}
const Menu& Menu::Run() const {
	frm.CallBack = CallBack;
	frm << static_cast<const graphics::Window&>(*this);
	frm.Swap();
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
	}
	return *this;
}
Menu::Data Menu::Adapter(const glm::ivec2 center, const std::vector<std::wstring> labels) {
	glm::ivec2 size{ fnt.MaxLeingth(labels) };
	glm::ivec4 rectangle{};
	return { center, size, rectangle, std::move(labels) };
}
std::vector<std::tuple<glm::vec2, glm::vec2>> Region() {

}
Menu::Menu(const Data data) :
	graphics::Window{
		graphics::vertex::Array::GetRectangle(data.rectangle)
	},
	std::vector<Button>{},
	border{ center, std::invoke([&]() {
		size = fnt.MaxLeingth(labels);
		return glm::ivec2{ size.x + size.y + size.y, (labels.size() * size.y * 5 >> 2) + (size.y >> 2) };
		}) }
{}
/*
void Menu::CallBack() {
	if (border == frm.Cursor()) {
		if (false) {
			;
		}
		else 
			static_cast<graphics::uniforms::Flash*>(this)->Set(true);
	}
	else {
		static_cast<graphics::uniforms::Flash*>(this)->Set(false);
	}
	frm << *this;
	for (const auto& button : *this) {
		frm << button;
	}
	frm.Swap();
}
*/