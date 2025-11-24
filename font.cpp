#include <Windows.h>
#include <map>
#include "font.h"
#include "resource.h"

#define FT_FREETYPE_H
#include "../freetype/include/freetype/ftglyph.h"

namespace font {
	struct Glyph {
		const bitmap::Bitmap<bitmap::Red> bitmap;
		const glm::ivec2 pos;
		const glm::ivec2 advance;

		Glyph() = default;
		Glyph(Glyph&&) = default;
		Glyph(const Glyph& glyph) = default;
		Glyph(const FT_GlyphSlot glyph);
	};
	Glyph::Glyph(const FT_GlyphSlot glyph) :
		bitmap{
			glm::ivec2{ glyph->bitmap.width, glyph->bitmap.rows },
			static_cast<bitmap::Red*>(static_cast<void*>(glyph->bitmap.buffer))
	},
		pos{ glyph->bitmap_left, glyph->bitmap_top },
		advance{ glyph->advance.x >> 6, glyph->advance.y >> 6 }
	{}

	constexpr int default_dpi = 72;
	glm::ivec2 dpi{ default_dpi, default_dpi };
	Fonts& SetDPI(const glm::ivec2 new_dpi) {
		dpi = new_dpi;
		return Fonts::fnt;
	}

	FT_Library const library{ std::invoke([]() {
		FT_Library library;
		FT_Init_FreeType(&library);
		return library;
	}) };

	struct Glyphs :
		std::map<unsigned, Glyph>
	{};

	struct Font {
	public:
		FT_Face const face;
		const float height;
		const float under;
		std::map<unsigned, Glyphs> glyphs;
	public:
		Font(FT_Face const face);
	};
	Font::Font(FT_Face const face) :
		face{ face },
		height{ (float)face->bbox.yMax / face->units_per_EM * 1.15f },
		under{ -(float)face->bbox.yMin / face->units_per_EM }
	{}
	Fonts::Fonts() {
		SetDPI({ 96,96 });
		LoadFont(IDR_FONT1);
		SetFont(IDR_FONT1);
		SetHeight(12);
	}

	FT_Face LoadFace(const unsigned ID) {
		FT_Face face;
		auto resource = FindResource(NULL, MAKEINTRESOURCE(ID), RT_FONT);
		auto data{ LockResource(LoadResource(NULL, resource)) };
		auto size{ SizeofResource(NULL, resource) };
		FT_New_Memory_Face(library, static_cast<const unsigned char*>(data), size, 0, &face);
		return face;
	}

	std::map<unsigned, Font> fonts;
	Font* font = nullptr;

	Fonts& LoadFont(const unsigned ID) {
		if (fonts.end() == fonts.find(ID))
			font = &fonts.emplace(ID, LoadFace(ID)).first->second;
		return Fonts::fnt;
	}

	constexpr unsigned default_height = 12;
	unsigned height = default_height;

	Fonts& SetFont(const unsigned ID) {
		if (auto iter{ fonts.find(ID) }; fonts.end() != iter)
			font = &iter->second;
		return Fonts::fnt;
	}
	Fonts& SetHeight(const unsigned new_height) {
		height = new_height;
		FT_Set_Char_Size(font->face, 0, height << 6, dpi.x, dpi.y);
		return Fonts::fnt;
	}
	/*
	Font::~Font() {
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
	*/
	glm::ivec2 GetSize(const std::wstring& text) {
		int length = 0;
		auto& glyphs{ font->glyphs[height] };
		for (auto ch : text) {
			auto index{ FT_Get_Char_Index(font->face, ch) };
			auto iter{ glyphs.find(index) };
			if (glyphs.end() == iter) {
				FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
				FT_Render_Glyph(font->face->glyph, FT_RENDER_MODE_NORMAL);
				iter = glyphs.emplace(index, Glyph{ font->face->glyph }).first;
			}
			length += iter->second.advance.x;
		}
		return { length, (font->height + font->under) * height };
	}

	glm::ivec2 MaxSize(const std::vector<std::wstring>& vector) {
		glm::ivec2 max{ 0,0 };
		for (auto& elem : vector) {
			auto size{ GetSize(elem) };
			max.x = max.x > size.x ? max.x : size.x;
			max.y = max.y > size.y ? max.y : size.y;
		}
		return max;
	}
	Red GetBitmap(const std::wstring& text) {
		return GetBitmap(text, GetSize(text));
	}
	Red GetBitmap(const std::wstring& text, glm::ivec2 size) {
		const glm::ivec2 native{ GetSize(text) };
		glm::ivec2 offset{
			(size.x - native.x + 1) >> 1,
			(size.y - native.y + 1) >> 1
		};
		return GetBitmap(text, size, offset);
	}
	Red GetBitmap(const std::wstring& text, glm::ivec2 size, glm::ivec2 offset) {
		Red bitmap{ size };
		auto& glyphs{ font->glyphs[height] };
		for (auto ch : text) {
			auto& glyph = glyphs.at(FT_Get_Char_Index(font->face, ch));
			bitmap.Insert(glyph.bitmap, { offset.x + glyph.pos.x, offset.y + font->height * height - glyph.pos.y });
			offset.x += glyph.advance.x;
		}
		bitmap.MirrorXX();
		return bitmap;
	}
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts) {
		return GetVecBitmap(texts, MaxSize(texts));
	}
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size) {
		const glm::ivec2 native{ MaxSize(texts) };
		glm::ivec2 offset{
			(size.x - native.x + 1) >> 1,
			(size.y - native.y + 1) >> 1
		};
		return GetVecBitmap(texts, size, offset);
	}
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size,
		glm::ivec2 offset) {
		std::vector<Red> result;
		for (auto& text : texts) {
			result.push_back(GetBitmap(text, size, offset));
		}
		return result;
	}
	glm::ivec2 Center(const glm::ivec2 size, const glm::ivec2 native) {
		return glm::ivec2{
			(size.x - native.x + 1) >> 1,
			(size.y - native.y + 1) >> 1
		};
	}
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size,
		glm::ivec2(*offset)(const glm::ivec2 size, const glm::ivec2 native)) {
		std::vector<Red> result;
		for (auto& text : texts) {
			result.push_back(GetBitmap(text, size, offset(size, GetSize(text))));
		}
		return result;
	}
}
