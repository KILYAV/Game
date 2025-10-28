#pragma once

namespace setting {
	class Setting {
	public:
		struct Scale {
			float menu = 1.5;
			float button = 1.0;
		} const scale;
	public:
		struct Font {
			unsigned ID;
			unsigned height;
		} const font;
	public:
		Setting(Setting&&) = delete;
		Setting(const Setting&) = delete;
		~Setting();

		void operator= (Setting&& setting) = delete;

		static Setting stt;
	private:
		Setting();
	};
#ifdef LEON_OPENGL_IMPLEMENTATION
	Setting Setting::stt;
#endif
}