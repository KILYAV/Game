#include <Windows.h>
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "graphic.h"
#include "button.h"

setting::Setting setting::Setting::stt;
frame::Frame frame::Frame::frm;
font::Font font::Font::fnt;

void Callback() {

}
int main()
{
	button::Exit exit;

	//frm.CallBack = nullptr;
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
		exit.Draw();
		frm.Swap();
	}
}