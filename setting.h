#pragma once

namespace setting {
	class Setting {
	private:
		//bool full_screen = false;
	public:
		struct Font {
			unsigned ID;
			unsigned height;
		};
		const Font font;
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