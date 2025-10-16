#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <Shlobj.h>
#include "setting.h"

using St = setting::Setting;

std::filesystem::path path;
void Init(St* setting) {
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
		file.read(static_cast<char*>(static_cast<void*>(setting)), sizeof(St));
		file.close();
	}
}
constexpr wchar_t default_font_name[] = L"Arial";
constexpr unsigned default_font_height = 12;
St::Setting() :
	font{ default_font_name, default_font_height }
{}
St::~Setting() {
	std::fstream file{ path };
	file.write(static_cast<const char*>(static_cast<const void*>(this)), sizeof(Setting));
	file.close();
}