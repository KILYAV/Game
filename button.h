#pragma once
#include <optional>
#include "frame.h"
#include "graphic.h"

namespace button {
	template<class Texture>
	class Button :
		Texture,
		graphic::object::Rectangle<Texture>
	{
	public:
		const glm::ivec4 region;

		using Rectangle = graphic::object::Rectangle<Texture>;
		using Rectangle::Draw;
		Button(const std::optional<glm::ivec2> pos = std::nullopt,
			const std::optional<glm::ivec2> size = std::nullopt);
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
		},
		region{}
	{}

	using Exit = Button<graphic::texture::button::Exit>;
}