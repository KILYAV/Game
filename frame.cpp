#include "frame.h"
#include "setting.h"

#define FULLSCREEN
#undef FULLSCREEN

namespace window {
	Window::Window() :
		Window{ std::invoke([]() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			return glfwGetPrimaryMonitor();
			})}
	{}
	Window::Window(GLFWmonitor* const monitor) :
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
			size.center = { size.integer.x >> 1, size.integer.y >> 1 };
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
		glfwSetCursorPosCallback(window, CursorPosCallBack);
		glfwSetMouseButtonCallback(window, MouseCallBack);
	}
	Window::~Window() {
		glfwTerminate();
	}
	void Window::CursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
		void* instant{ glfwGetWindowUserPointer(window) };
		CallBack(instant, glm::ivec2{ xpos - frm.size.center.x, ypos - frm.size.center.y }, std::nullopt);
		CallPaint(instant);
		glfwSwapBuffers(window);
	}
	void Window::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		void* instant{ glfwGetWindowUserPointer(window) };
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		CallBack(instant, glm::ivec2{ xpos - frm.size.center.x, ypos - frm.size.center.y },
			std::tuple{ button, action, mods });
		CallPaint(instant);
		glfwSwapBuffers(window);
	}
}