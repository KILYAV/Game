#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
//#include "graphics.h"

namespace frame {
	class Frame {
	private:
		GLFWmonitor* const monitor;
	public:
		struct Size {
			glm::vec2 floating;
			glm::ivec2 integer;
			glm::ivec2 physical;
			glm::ivec2 dpi;
		} const size;
		GLFWwindow* const window;
	private:
		glm::vec2 cursor{ 0, 0 };
		struct Mouse {
			int button;
			int action;
			int mods;
		} mouse{ 0, 0, 0 };
	public:
		glm::ivec2 Cursor() const {
			return cursor;
		}
		void (*CallBack)() { DefaultCallBack };
	public:
		static Frame frm;

		Frame(Frame&&) = delete;
		Frame(const Frame&) = delete;
		~Frame();

		//const Frame& operator<< (const graphics::Graphics& grph) const;
		const Frame& Swap() const;
	private:
		Frame();
		static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
		static void DefaultCallBack() {};
	};
}
static frame::Frame& frm = frame::Frame::frm;