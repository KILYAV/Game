#pragma once
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
//#include "graphics.h"

namespace frame {
	class Frame {
	private:
		GLFWmonitor* const monitor;
	public:
		struct Size {
			glm::vec2 pixel;
			glm::vec2 floating;
			glm::ivec2 integer;
			glm::ivec2 physical;
			glm::ivec2 dpi;
		} const size;
		GLFWwindow* const window;
		struct Mouse {
			int button;
			int action;
			int mods;
		};
	private:
		glm::ivec2 cursor{ 0, 0 };
		Mouse mouse{ 0, 0, 0 };
	public:
		glm::ivec2 Cursor() const {
			return cursor;
		}
		glm::ivec2 Center() const {
			return { size.integer.x / 2, size.integer.y / 2 };
		}
		void* instant = nullptr;
		void (*CallBackCursorPos)(void*, glm::ivec2) = nullptr;
		void (*CallBackMouse)(void*) = nullptr;
	public:
		static Frame frm;

		Frame(Frame&&) = delete;
		Frame(const Frame&) = delete;
		~Frame();

		const Frame& Swap() const;
	private:
		Frame();
		static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
	};
}
static frame::Frame& frm = frame::Frame::frm;