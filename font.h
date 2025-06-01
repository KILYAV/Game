#pragma once
#include <map>
#include <vector>
#include <string>
#include "bitmap.h"
#include "freetype/ftglyph.h"

class Font;
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
	class Glyphes :
		std::map<unsigned, Glyph>
	{
	public:
		Glyphes(Glyphes&&) = default;
		Glyphes(const Glyphes&) = default;
		Glyphes(const FT_Face face, const unsigned height);
		int Leingth(const FT_Face face, const std::wstring& text);
		std::map<unsigned, Glyph>::at;
		const unsigned height;
		const unsigned under;
	};
	struct FontBase {
		FT_Library library;
		FT_Face face;
		FontBase();
	};
	class Font :
		FontBase,
		std::map<unsigned, Glyphes>
	{
	private:
		Glyphes* glyphes;
	public:
		Font(Font&&) = delete;
		Font(const Font&) = delete;
		~Font();

		Font& SetHeight(unsigned height);
		unsigned Leingth(const std::wstring& text);
		glm::ivec2 MaxLeingth(const std::vector<std::wstring>& texts);
		bitmap::BitMap<bitmap::Red> GetBitMap(const std::wstring& text) const;

		static Font fnt;
	private:
		Font();
	};
}
static font::Font& fnt = font::Font::fnt;