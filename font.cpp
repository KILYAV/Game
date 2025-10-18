#include <Windows.h>
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "resource.h"
#include FT_FREETYPE_H

namespace font {
	Glyph::Glyph(const Glyph& glyph) :
		bitmap::BitMap<bitmap::Red>{ static_cast<const bitmap::BitMap<bitmap::Red>&>(glyph) },
		pos{ glyph.pos },
		advance{ glyph.advance }
	{}
	Glyph::Glyph(const FT_GlyphSlot glyph) :
		bitmap::BitMap<bitmap::Red>{
			static_cast<bitmap::Red*>(static_cast<void*>(glyph->bitmap.buffer)),
			glm::ivec2{ glyph->bitmap.width, glyph->bitmap.rows }
	},
		pos{ glyph->bitmap_left, glyph->bitmap_top },
		advance{ glyph->advance.x >> 6, glyph->advance.y >> 6 }
	{}

	constexpr unsigned default_dpi = 72;
	Glyphs::Glyphs(const FT_Face face, const unsigned new_height) :
		std::map<unsigned, Glyph>{},
		height{ (face->bbox.yMax - face->bbox.yMin) * new_height * frm.size.dpi.y / face->units_per_EM / default_dpi },
		under{ face->bbox.yMax * new_height * frm.size.dpi.y / face->units_per_EM / default_dpi }
	{}
	glm::ivec2 Glyphs::Size(const FT_Face face, const std::wstring& text) {
		int length = height;
		for (auto ch : text) {
			auto index{ FT_Get_Char_Index(face, ch) };
			auto iter{ this->find(index) };
			if (this->end() == iter) {
				FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
				FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
				iter = this->emplace(index, Glyph{ face->glyph }).first;
			}
			length += iter->second.advance.x;
		}
		return { length, height };
	}

	namespace font {
		Base::Base() :
			library{ std::invoke([]() {
				FT_Library library;
				FT_Init_FreeType(&library);
				return library;
				}) },
			face{ std::invoke([&]() {
				auto resource = FindResource(NULL, MAKEINTRESOURCE(stt.font.ID), RT_FONT);
				auto data{ LockResource(LoadResource(NULL, resource)) };
				auto size{ SizeofResource(NULL, resource) };
				FT_Face face;
				FT_New_Memory_Face(library, static_cast<const unsigned char*>(data), size, 0, &face);
				return face;
				}) }
		{}
	}

	Font::Font() :
		font::Base{},
		std::map<unsigned, Glyphs>{ { stt.font.height, Glyphs{ face, stt.font.height } } },
		glyphes{ &this->begin()->second }
	{
		FT_Set_Char_Size(face, 0, stt.font.height << 6, frm.size.dpi.x, frm.size.dpi.y);
	}
	Font::~Font() {
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
	Font& Font::SetHeight(unsigned height) {
		if (false == this->contains(height)) {
			this->emplace(height, Glyphs{ face, height });
		}
		FT_Set_Char_Size(face, 0, height, frm.size.dpi.x, frm.size.dpi.y);
		return *this;
	}
	glm::ivec2 Font::Leingth(const std::wstring& text) {
		return glyphes->Size(face, text);
	}
	glm::ivec2 Font::MaxLeingth(const std::vector<std::wstring>& texts) {
		unsigned lenght = 0;
		for (auto& text : texts) {
			int temp{ glyphes->Size(face,text).x };
			lenght = lenght > temp ? lenght : temp;
		}
		return { lenght,glyphes->height };
	}
	bitmap::BitMap<bitmap::Red> Font::GetBitMap(const std::wstring& text) const {
		bitmap::BitMap<bitmap::Red> bitmap{ nullptr, glyphes->Size(face, text) };
		unsigned length = 0;
		for (auto ch : text) {
			auto& glyph = glyphes->at(FT_Get_Char_Index(face, ch));
			bitmap.Insert(glyph, { length + glyph.pos.x, glyphes->under - glyph.pos.y });
			length += glyph.advance.x;
		}
		return bitmap;
	}
}