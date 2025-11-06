#include "frame.h"

namespace frame {
	Frame::Frame() :
		monitor{ std::invoke([]() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			return glfwGetPrimaryMonitor();
			})},
		size{ GetSize() },
		window{ GetWindow() }
	{
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, CursorPosCallBack);
		glfwSetMouseButtonCallback(window, MouseCallBack);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glClearColor(0., 0., 0., 1.0f);
		glfwSwapBuffers(window);
	}
	Frame::~Frame() {
		glfwTerminate();
	}

	void Frame::CursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
		Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
		(*frame.*frame->call_back)(
			glm::ivec2{ xpos - frame->size.center.x, frame->size.center.y - ypos }, std::nullopt);
		(*frame.*frame->call_paint)();
		glfwSwapBuffers(window);
	}
	void Frame::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		(*frame.*frame->call_back)(glm::ivec2{ xpos - frame->size.center.x, frame->size.center.y - ypos},
			std::tuple{ button, action, mods });
		(*frame.*frame->call_paint)();
		glfwSwapBuffers(window);
	} 
	Frame::Size_t Frame::GetSize() {
		Size_t size;
		auto mode{ glfwGetVideoMode(monitor) };
		glfwGetMonitorPhysicalSize(monitor, &size.physical.x, &size.physical.y);
		if (setting::Setting::stt.FullScreen()) {
			size.integer.x = mode->width;
			size.integer.y = mode->height;
		}
		else {
			size.integer = setting::Setting::stt.Size();
			size.physical.x = size.physical.x * size.integer.x / mode->width;
			size.physical.y = size.physical.y * size.integer.y / mode->height;
		}
		size.floating = size.integer;
		size.center = { size.integer.x >> 1, size.integer.y >> 1 };
		size.pixel = { 1. / size.integer.x, 1. / size.integer.y };
		constexpr int mm_in_inch = 254;
		size.dpi.x = size.integer.x * mm_in_inch / (size.physical.x * 10);
		size.dpi.y = size.integer.y * mm_in_inch / (size.physical.y * 10);
		return size;
	}
	GLFWwindow* Frame::GetWindow() {
		GLFWwindow* window;
			if (setting::Setting::stt.FullScreen()) {
				window = glfwCreateWindow(size.integer.x, size.integer.y, "The Rose and the Worm", NULL, NULL);
				glfwSetWindowMonitor(window, monitor, 0, 0, size.integer.x, size.integer.y, GLFW_DONT_CARE);
			}
			else
				window = glfwCreateWindow(size.integer.x, size.integer.y, "TEST", NULL, NULL);
			return window;
	}
	void Frame::FullScreen() {
		glfwSetWindowMonitor(window, monitor, 0, 0, size.integer.x, size.integer.y, GLFW_DONT_CARE);

		//glfwSetWindowSize(window, size.integer.x << 1, size.integer.y << 1);
	}
}