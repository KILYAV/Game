#pragma once
#include <Windows.h>
#include <variant>
#include "../glm/glm/glm.hpp"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "setting.h"

namespace window {
	struct Window {
		struct Size {
			glm::vec2 pixel;
			glm::vec2 floating;
			glm::ivec2 integer;
			glm::ivec2 physical;
			glm::ivec2 center;
			glm::ivec2 dpi;
		} const size;
	private:
		GLFWwindow* window;
		inline static void (*CallBack)(void*, std::variant<glm::ivec2, std::tuple<int,int,int>>);
		static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
		Window();
		Window(GLFWmonitor* const monitor);
	public:
		static Window frm;

		Window(Window&&) = delete;
		Window(const Window&) = delete;
		~Window();

		template<class Object>
		Window& operator<< (Object& object);
	};
	template<class Object>
	Window& Window::operator<< (Object& object) {
		glfwSetWindowUserPointer(window, &object);
		CallBack = &object.CallBack;

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0)) {
			DispatchMessage(&msg);
			object.Draw();
			glfwSwapBuffers(window);
		}
		return *this;
	}
#ifdef LEON_OPENGL_IMPLEMENTATION
	Window Window::frm;
#endif
}