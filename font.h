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
	class Glyphs :
		std::map<unsigned, Glyph>
	{
	public:
		Glyphs(Glyphs&&) = default;
		Glyphs(const Glyphs&) = default;
		Glyphs(const FT_Face face, const unsigned height);
		glm::ivec2 Size(const FT_Face face, const std::wstring& text);
		using std::map<unsigned, Glyph>::at;
		const unsigned height;
		const unsigned under;
	};
	namespace font {
		struct Base {
			FT_Library library;
			FT_Face face;
			Base();
		};
	}
	class Font :
		font::Base,
		std::map<unsigned, Glyphs>
	{
	private:
		Glyphs* glyphes;
	public:
		Font(Font&&) = delete;
		Font(const Font&) = delete;
		~Font();

		Font& SetHeight(unsigned height);
		glm::ivec2 Leingth(const std::wstring& text);
		glm::ivec2 MaxLeingth(const std::vector<std::wstring>& texts);
		bitmap::BitMap<bitmap::Red> GetBitMap(const std::wstring& text) const;

		static Font fnt;
	private:
		Font();
	};
}

static font::Font& fnt = font::Font::fnt;