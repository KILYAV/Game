#include <fstream>
#include <filesystem>
#include <Shlobj.h>
#include "resource.h"
#include "setting.h"

namespace setting {
	std::filesystem::path path{ std::invoke([]() {
		PWSTR buff;
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &buff);
		std::filesystem::path path{ std::filesystem::path{ buff } / "TRATW" };
		CoTaskMemFree(buff);
		if (false == std::filesystem::exists(path)) {
			create_directory(path);
		}
		return path / "stt";
		}) };

	Setting::Setting()	:
		Frame_t{},
		font{ IDR_FONT1, 14 }
	{
		if (std::filesystem::exists(path)) {
			std::fstream file{ path };
			file.read(static_cast<char*>(static_cast<void*>(this)), sizeof(Setting));
			file.close();
		}
	}
	Setting::~Setting() {
		std::fstream file{ path };
		file.write(static_cast<const char*>(static_cast<const void*>(this)), sizeof(Setting));
		file.close();
	}
}