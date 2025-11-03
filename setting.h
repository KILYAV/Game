#pragma once
#include "../glm/glm/glm.hpp"

namespace setting {
	struct Setting {
	public:
		struct Frame_t {
			bool full_screen;
			glm::ivec2 size;
		};
		struct Font_t {
			unsigned ID;
			unsigned height;
		};
	private:
		Frame_t frame;
		Font_t font;
	public:
		const Frame_t& Frame() const {
			return frame;
		}
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