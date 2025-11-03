#pragma once
#include <Windows.h>
#include "message.h"

namespace function {
	namespace main {
		inline bool Message(int,int) {
			return false;
		}
	}
	namespace cancel_apply {
		inline bool CancelApply(int, int) {
			return false;
		}
	}
	namespace setting {
		namespace graphic {
			inline bool Graphic(int, int) {
				return false;
			}
		}
		inline bool Setting(int, int) {
			return false;
		}
	}
}