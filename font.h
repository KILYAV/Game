#pragma once
#include <map>
#include <vector>
#include <string>
#include "bitmap.h"
#include "setting.h"
#include "frame.h"
#include "../freetype/include/freetype/ftglyph.h"

namespace font {
	struct Glyph :
		bitmap::BitMap<bitmap::Red>
	{
		Glyph() = default;
		Glyph(Glyph&&) = default;
		Glyph(const Glyph&);
		Glyph(const FT_GlyphSlot glyph);
		const glm::ivec2 pos;
		const glm::ivec2 advance;
	};
	struct Face :
		std::map<unsigned, Glyph>
	{
	public:
		const unsigned height;
		const unsigned under;
		Face(const FT_Face face, const unsigned new_height);
	};

	class Font :
		std::map<unsigned, Face>
	{
	private:
		std::map<unsigned, Face>::iterator glyphs;
		FT_Library const library;
		FT_Face face;
	private:
		Font();
		Font(Font&&) = delete;
		Font(const Font&) = delete;
		~Font();
	public:
		unsigned Height();
		unsigned Height(const unsigned height);
		glm::ivec2 Size(const std::wstring& text);
		bitmap::BitMap<bitmap::Red> GetBitMap(const std::wstring& text);

		static Font fnt;
	};
	glm::ivec2 MaxSize(const std::vector<std::wstring>& vector);
#ifdef LEON_OPENGL_IMPLEMENTATION
	Font Font::fnt;
#endif
}