#define LEON_OPENGL_IMPLEMENTATION
#include "setting.h"
#include "frame.h"
#include "font.h"
#include "menu.h"

#include "message.h"

int main()
{
	menu::Main main;
	menu::Setting setting;
	
	int message;
	do {
		message = frame::Frame::frm.Run(main);
		switch (message) {
		case (UM_BUTTON_SETTING): frame::Frame::frm.Run(setting); break;
		}
	} while (UM_BUTTON_EXIT != message);
}