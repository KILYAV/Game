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
			glm::ivec2{ glyph->bitmap.width, glyph->bitmap.rows },
			true
	},
		pos{ glyph->bitmap_left, glyph->bitmap_top },
		advance{ glyph->advance.x >> 6, glyph->advance.y >> 6 }
	{}

	constexpr unsigned default_dpi = 72;
	Face::Face(const FT_Face face, const unsigned new_height) :
		std::map<unsigned, Glyph>{},
		height{ (face->bbox.yMax - face->bbox.yMin) * new_height * frame::Frame::frm.Size().dpi.y
			/ face->units_per_EM / default_dpi },
		under{ face->bbox.yMax * new_height * frame::Frame::frm.Size().dpi.y / face->units_per_EM / default_dpi }
	{}

	Font::Font() :
		library{ std::invoke([&]() {
			FT_Library library;
			FT_Init_FreeType(&library);
			return library;
			})}
	{
		auto resource = FindResource(NULL, MAKEINTRESOURCE(setting::Setting::stt.Font().ID), RT_FONT);
		auto data{ LockResource(LoadResource(NULL, resource)) };
		auto size{ SizeofResource(NULL, resource) };
		FT_New_Memory_Face(library, static_cast<const unsigned char*>(data), size, 0, &face);
		FT_Set_Char_Size(face, 0, setting::Setting::stt.Font().height << 6,
			frame::Frame::frm.Size().dpi.x, frame::Frame::frm.Size().dpi.y);
		glyphs = this->emplace(setting::Setting::stt.Font().height,
			Face{ face, setting::Setting::stt.Font().height }).first;
	}
	Font::~Font() {
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
	unsigned Font::Height() {
		return glyphs->second.height;
	}
	unsigned Font::Height(const unsigned height) {
		if (false == this->contains(height)) {
			this->emplace(height, Face{ face, height } );
		}
		FT_Set_Char_Size(face, 0, height, frame::Frame::frm.Size().dpi.x, frame::Frame::frm.Size().dpi.y);
		return height;
	}
	glm::ivec2 Font::Size(const std::wstring& text) {
		int length = 0;
		for (auto ch : text) {
			auto index{ FT_Get_Char_Index(face, ch) };
			auto iter{ glyphs->second.find(index) };
			if (glyphs->second.end() == iter) {
				FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
				FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
				iter = glyphs->second.emplace(index, Glyph{ face->glyph }).first;
			}
			length += iter->second.advance.x;
		}
		return { length, glyphs->second.height };
	}
	bitmap::BitMap<bitmap::Red> Font::GetBitMap(const std::wstring& text) {
		bitmap::BitMap<bitmap::Red> bitmap{ nullptr, Size(text) };
		unsigned length = 0;
		for (auto ch : text) {
			auto& glyph = glyphs->second.at(FT_Get_Char_Index(face, ch));
			bitmap.Insert(glyph, { length + glyph.pos.x, glyphs->second.under - glyph.pos.y });
			length += glyph.advance.x;
		}
		return bitmap;
	}
}