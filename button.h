#pragma once
#include <optional>
#include "frame.h"
#include "graphic.h"

namespace button {
	template<class Texture>
	struct Button :
		Texture,
		graphic::object::Rectangle<Texture>
	{
	public:
		using Rectangle = graphic::object::Rectangle<Texture>;
		using Rectangle::Draw;
		Button(const std::optional<glm::ivec2> pos = std::nullopt,
			const std::optional<glm::ivec2> size = std::nullopt);
		void Frm(frame::Frame& frm) {
			frm.CallBackCursorPos = CallBackCursorPos;
			frm.CallBackMouse = CallBackMouse;
			frm.instant = this;
		}
	private:
		static void CallBackCursorPos(void* instant, const glm::ivec2 pos) {
			static_cast<Button<Texture>*>(instant)->CallBack(pos);
		}
		static void CallBackMouse(void* instant) {
			static_cast<Button<Texture>*>(instant)->CallBack();
		}
		void CallBack(const glm::ivec2 pos) {
			if (Rectangle::Region(pos))
				Rectangle::Invert(true);
			else
				Rectangle::Invert(false);
		}
		void CallBack() {
			;
		}
	};
	template<class Texture>
	Button<Texture>::Button(const std::optional<glm::ivec2> pos,
		const std::optional<glm::ivec2> size) :
		Texture{},
		Rectangle{
			std::array<glm::ivec2, 4>{
				pos ? pos.value() : glm::ivec2{},
				size ? size.value() : Texture::texture.size,
				glm::ivec2{},
				Texture::texture.size
			}
		}
	{}

	using Exit = Button<graphic::texture::button::Exit>;
}