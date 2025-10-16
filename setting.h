#pragma once

namespace setting {
	class Setting {
	public:
		struct Font {
			const wchar_t* name;
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
}
static setting::Setting& stt = setting::Setting::stt;