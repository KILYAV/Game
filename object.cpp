#include <Windows.h>
#include <string>
#include "bitmap.h"
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "object.h"

using namespace object;
Button::Button(const wchar_t* label, const glm::vec2 center, const glm::vec2 size) :
	graphics::Window{
		graphics::vertex::Rectangle(glm::Rectangle(center,size)),
		bitmap::BitMap<bitmap::Red>{ nullptr, size }.Fill({ 0 })
			.Insert(fnt.GetBitMap(label), { (size.x - fnt.Leingth(label)) / 2 ,0 })
	},
	border{ center, size }
{
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
Menu::Data Menu::Init(const glm::vec2 center, const std::vector<std::wstring> labels) {
	glm::vec2 size{ fnt.MaxLeingth(labels) };
	glm::vec4 rectangle{ glm::Rectangle(center,
		{ size.x + size.y, labels.size() * size.y * 1.25 + size.y * .5 }) };
	return { center, size, rectangle, std::move(labels) };
}
Menu::Menu(const Data data) :
	graphics::Window{
		graphics::vertex::Rectangle(data.rectangle),
		bitmap::BitMap<bitmap::Red>{ nullptr, {0,0} }
	},
	std::vector<Button>{},
	border{ data.rectangle }
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