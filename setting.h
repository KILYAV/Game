#pragma once
#include "../glm/glm/glm.hpp"

namespace setting {
	struct Frame_t {
		Frame_t() :
			full_screen{ false },
			size{ 640, 480 }
		{}
		bool FullScreen() const {
			return full_screen;
		}
		bool FullScreen(bool value) {
			bool temp = full_screen;
			full_screen = value;
			return temp;
		}
		glm::ivec2 Size() const {
			return size;
		}
	protected:
		bool full_screen;
		glm::ivec2 size;
	};
	struct Setting :
		Frame_t
	{
	public:
		struct Font_t {
			unsigned ID;
			unsigned height;
		};
	private:
		Font_t font;
	public:
		const Font_t& Font() const {
			return font;
		}

	private:
		Setting();
		Setting(Setting&&) = delete;
		Setting(const Setting&) = delete;
		~Setting();

		void operator= (Setting&& setting) = delete;
	public:
		static Setting stt;
	};
#ifdef LEON_OPENGL_IMPLEMENTATION
	Setting Setting::stt;
#endif
}