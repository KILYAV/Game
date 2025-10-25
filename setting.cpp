#include <fstream>
#include <filesystem>
#include <Shlobj.h>
#include <knownfolders.h>
#include "resource.h"
#include "setting.h"

namespace setting {
	std::filesystem::path path;
	void Init(Setting* setting) {
		PWSTR buff;
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &buff);
		path = std::filesystem::path{ buff } / "TRATW";
		CoTaskMemFree(buff);
		if (false == std::filesystem::exists(path)) {
			create_directory(path);
		}
		path = path / "setting.stt";
		if (std::filesystem::exists(path)) {
			std::fstream file{ path };
			file.read(static_cast<char*>(static_cast<void*>(setting)), sizeof(Setting));
			file.close();
		}
	}
	constexpr unsigned Arial = IDR_FONT1;
	constexpr unsigned default_font_name = Arial;
	constexpr unsigned default_font_height = 12;
	Setting::Setting() :
		font{ default_font_name, default_font_height }
	{}
	Setting::~Setting() {
		std::fstream file{ path };
		file.write(static_cast<const char*>(static_cast<const void*>(this)), sizeof(Setting));
		file.close();
	}
}