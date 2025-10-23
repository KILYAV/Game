#include "frame.h"
#include "setting.h"

#define FULLSCREEN
#undef FULLSCREEN

namespace frame {
	Frame::Frame() :
		monitor{ std::invoke([]() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			return glfwGetPrimaryMonitor();
			}) },
		size{ std::invoke([&]() {
			Size size;
	#ifdef FULLSCREEN
			auto mode{ glfwGetVideoMode(monitor) };
			glfwGetMonitorPhysicalSize(monitor, &size.physical.x, &size.physical.y);
			size.integer.x = mode->width;
			size.integer.y = mode->height;
	#else
			size.physical.x = 169; size.physical.y = 127;
			size.integer.x = 640;
			size.integer.y = 480;
	#endif
			size.floating = size.integer;
			size.pixel = { 1. / size.integer.x, 1. / size.integer.y };
			constexpr int mm_in_inch = 254;
			size.dpi.x = size.integer.x * mm_in_inch / (size.physical.x * 10);
			size.dpi.y = size.integer.y * mm_in_inch / (size.physical.y * 10);
			return size;
			}) },
		window{ std::invoke([&]() {
	#ifdef FULLSCREEN
			auto window{ glfwCreateWindow(size.integer.x, size.integer.y, "The Rose and the Worm", NULL, NULL) };
			glfwSetWindowMonitor(window, monitor, 0, 0, size.integer.x, size.integer.y, GLFW_DONT_CARE);
	#else
			auto window{ glfwCreateWindow(640, 480, "TEST", NULL, NULL) };
	#endif
			glfwMakeContextCurrent(window);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			return window;
			}) }
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, CursorPosCallBack);
		glfwSetMouseButtonCallback(window, MouseCallBack);
	}
	Frame::~Frame() {
		glfwTerminate();
	}
	const Frame& Frame::Swap() const {
		glfwSwapBuffers(window);
		return *this;
	}
	void Frame::CursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
		Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
		glm::ivec2 cursor{ xpos - (frm.size.floating.x * .5), ypos - (frm.size.floating.y * .5) };
		frame->CallBackCursorPos(frm.instant, cursor);
		frame->cursor = cursor;
	}
	void Frame::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
		frame->mouse = { button, action, mods };
		frame->CallBackMouse(frm.instant);
		/*
			PostQuitMessage(0);
		*/
	}
}