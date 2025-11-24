#pragma once
#include <Windows.h>
#include <optional>
#include <iostream>
#include "setting.h"
#include "message.h"
#include "../glad/include/glad/glad.h"
#include "../glfw/include/GLFW/glfw3.h"

struct frame {
private:
	struct Modes_t {
		const GLFWvidmode* const ptr;
		const int count;
	private:
		int index = 0;
	public:
		Modes_t(const Modes_t& modes) = default;
		Modes_t(GLFWmonitor* const monitor);
		Modes_t(const GLFWvidmode* const ptr_,
			const int count_) :
			ptr{ ptr_ },
			count{ count_ }
		{}
		const GLFWvidmode& Get() const {
			return ptr[index];
		}
		int Up() {
			return index = count == index + 1 ? 0 : ++index;
		}
	};
public:
	struct Size_t {
		glm::vec4 floating;
		glm::vec4 pixel;
		glm::ivec4 integer;
		glm::ivec4 center;
	public:
		Size_t(const GLFWvidmode mode);
		Size_t(const int width, const int height);

		void operator= (const Size_t& other);
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
	void (frame::* call_back)(glm::ivec2, std::optional<std::tuple<int, int, int>>);
	void (frame::* call_paint)();
public:
	static const Size_t& Size() {
		return frm.size;
	}
	static void UpMode();
	//void FullScreen();
private:
	frame();
	frame(GLFWmonitor* const monitor);
	frame(GLFWmonitor* const monitor, const Modes_t modes);
	frame(const Modes_t modes, const glm::ivec2 physical);
	frame(const Modes_t modes, const glm::ivec2 physical, const glm::vec2 dpi);
	frame(frame&&) = delete;
	frame(const frame&) = delete;
	~frame();
public:
	template<class Object>
	static int Run(Object& object);
private:
	template<class Object>
	void CallBack(glm::ivec2, std::optional<std::tuple<int, int, int>>);
	template<class Object>
	void CallPaint();

	static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
	static void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
public:
	static frame frm;
};
template<class Object>
int frame::Run(Object& object) {
	frm.instant = &object;
	frm.call_back = &frame::CallBack<Object>;
	frm.call_paint = &frame::CallPaint<Object>;

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(frm.window);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(frm.window);
	object.Paint();
	glfwSwapBuffers(frm.window);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (WM_USER == msg.message)
			if (false == object.CallMessage(msg.wParam, msg.lParam))
				break;
		DispatchMessage(&msg);
	}

	frm.instant = nullptr;
	frm.call_back = nullptr;
	frm.call_paint = nullptr;

	int result = msg.lParam;
	DispatchMessage(&msg);
	return result;
}
template<class Object>
void frame::CallBack(glm::ivec2 pos, std::optional<std::tuple<int, int, int>> mouse) {
	static_cast<Object*>(instant)->CallBack(pos, mouse);
}
template<class Object>
void frame::CallPaint() {
	static_cast<Object*>(instant)->Paint();
}
#ifdef LEON_OPENGL_IMPLEMENTATION
	frame frame::frm;
#endif