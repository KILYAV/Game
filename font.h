#pragma once
#include <string>
#include "bitmap.h"

namespace font {
	class Fonts {
	private:
		Fonts();
		Fonts(Fonts&&) = delete;
		Fonts(const Fonts&) = delete;
	public:
		static Fonts fnt;
		inline Fonts& operator<< (Fonts& fnt) {
			return fnt;
		}
	};

	Fonts& SetDPI(const glm::ivec2 dpi);
	Fonts& SetFont(const unsigned ID);
	Fonts& SetHeight(const unsigned height);

	Fonts& LoadFont(const unsigned ID);

	glm::ivec2 GetSize(const std::wstring& text);
	glm::ivec2 MaxSize(const std::vector<std::wstring>& vector);

	using Red = bitmap::Bitmap<bitmap::Red>;
	Red GetBitmap(const std::wstring& text);
	Red GetBitmap(const std::wstring& text, glm::ivec2 size);
	Red GetBitmap(const std::wstring& text, glm::ivec2 size, glm::ivec2 offset);

	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts);
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size);
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size, glm::ivec2 offset);

	glm::ivec2 Center(const glm::ivec2 native, const glm::ivec2 size);
	std::vector<Red> GetVecBitmap(const std::vector<std::wstring>& texts, glm::ivec2 size,
		glm::ivec2(*offset)(const glm::ivec2 native, const glm::ivec2 size));
#ifdef LEON_OPENGL_IMPLEMENTATION
	Fonts Fonts::fnt;
	Fonts& fnt = Fonts::fnt;
#endif
}