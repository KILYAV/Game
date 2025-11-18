#pragma once
#include <Windows.h>
#include <optional>
#include <iostream>
#include "setting.h"
#include "message.h"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"

namespace frame {
	struct Frame {
	public:
		struct Modes_t {
			const GLFWvidmode* const ptr;
			const int count;
		private:
			int index = 0;
		public:
			Modes_t(const GLFWvidmode* const ptr_,
				const int count_) :
				ptr{ ptr_ },
				count{ count_ }
			{}
			const GLFWvidmode& Get() const {
				return ptr[index];
			}
			const GLFWvidmode& Back() const {
				return ptr[count - 1];
			}
			int Up() {
				return index = count == index + 1 ? 0 : ++index;
			}
		};
		struct Size_t {
			glm::vec2 floating;
			glm::vec2 pixel;
			glm::ivec2 integer;
			glm::ivec2 center;
		};
	private:
		Modes_t modes;
		Size_t size;
	public:
		const glm::vec2 dpi;
	private:
		GLFWwindow* window;
		const unsigned uniform;

		void* instant;
		void (Frame::*call_back)(glm::ivec2, std::optional<std::tuple<int,int,int>>);
		void (Frame::*call_paint)();

		Size_t ReSize(const glm::ivec2 physical);
	public:
		const Size_t& Size() const {
			return size;
		}
		//void FullScreen();
	private:
		Frame();
		Frame(GLFWmonitor* const monitor);
		Frame(GLFWmonitor* const monitor, const Modes_t modes);
		Frame(const Modes_t modes, const glm::ivec2 physical);
		Frame(const Modes_t modes, const glm::ivec2 physical, const glm::vec2 dpi);
		Frame(Frame&&) = delete;
		Frame(const Frame&) = delete;
		~Frame();
	public:
		template<class Object>
		int Run(Object& object);
	private:
		template<class Object>
		void CallBack(glm::ivec2, std::optional<std::tuple<int, int, int>>);
		template<class Object>
		void CallPaint();

		static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
	public:
		static Frame frm;
	};
	template<class Object>
	int Frame::Run(Object& object) {
		instant = &object;
		call_back = &Frame::CallBack<Object>;
		call_paint = &Frame::CallPaint<Object>;

		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		object.Paint();
		glfwSwapBuffers(window);

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (WM_USER == msg.message)
				if (false == object.CallMessage(msg.wParam, msg.lParam))
					break;
			DispatchMessage(&msg);
		}

		instant = nullptr;
		call_back = nullptr;
		call_paint = nullptr;

		int result = msg.lParam;
		DispatchMessage(&msg);
		return result;
	}
	template<class Object>
	void Frame::CallBack(glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
		static_cast<Object*>(instant)->CallBack(pos, mouse);
	}
	template<class Object>
	void Frame::CallPaint() {
		static_cast<Object*>(instant)->Paint();
	}
#ifdef LEON_OPENGL_IMPLEMENTATION
	Frame Frame::frm;
#endif
}