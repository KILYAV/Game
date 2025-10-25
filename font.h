#pragma once
#include <map>
#include <vector>
#include <string>
#include "bitmap.h"
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
		FT_Library library;
		FT_Face face;
		std::map<unsigned, Face>::iterator glyphs;
	public:
		Font(Font&&) = delete;
		Font(const Font&) = delete;
		~Font();

		unsigned Height(const unsigned height = 0);
		glm::ivec2 Size(const std::wstring& text);
		bitmap::BitMap<bitmap::Red> GetBitMap(const std::wstring& text);

		static Font fnt;
	private:
		Font();
	};
#ifdef LEON_OPENGL_IMPLEMENTATION
	Font Font::fnt;
#endif
}