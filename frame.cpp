#include "frame.h"

namespace frame {
	Frame::Frame() :
		Frame{ std::invoke([]() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			return glfwGetPrimaryMonitor();
			}) }
	{}
	Frame::Frame(GLFWmonitor* monitor) :
		Frame{ monitor, std::invoke([&]() {
			int count;
			auto mode{ glfwGetVideoModes(monitor, &count) };
			return Modes_t{ mode, count };
			}) }
	{}
	Frame::Frame(GLFWmonitor* const monitor, const Modes_t modes) :
		Frame{ modes, std::invoke([&]() {
			int X,Y;
			glfwGetMonitorPhysicalSize(monitor, &X, &Y);
			return glm::ivec2{ X,Y };
			}) }
	{}
	Frame::Frame(const Modes_t modes, const glm::ivec2 physical) :
		Frame{ modes, physical, std::invoke([&]() {
			glm::vec2 dpi;
			auto& mode{ modes.ptr[modes.count - 1] };
			constexpr int mm_in_inch = 254;
			dpi.x = mode.width * mm_in_inch / (physical.x * 10);
			dpi.y = mode.height * mm_in_inch / (physical.y * 10);
			return dpi;
			})
		}
	{};
	Frame::Frame(const Modes_t modes_, const glm::ivec2 physical_, const glm::vec2 dpi_) :
		modes{ modes_ },
		size{ ReSize(physical_) },
		dpi{ dpi_ },
		window{ glfwCreateWindow(size.integer.x, size.integer.y, "The Rose and the Worm", NULL, NULL) },
		uniform{ std::invoke([&]() {
			glfwMakeContextCurrent(window);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

			constexpr unsigned index = 0;
			unsigned block = 0;
			glGenBuffers(1, &block);
			glBindBuffer(GL_UNIFORM_BUFFER, block);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Size_t), &size, GL_STATIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, block);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			return block;
			})}
	{
		glfwSetWindowSize(window, size.integer.x, size.integer.y);

		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, CursorPosCallBack);
		glfwSetMouseButtonCallback(window, MouseCallBack);

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
	Frame::Size_t Frame::ReSize(const glm::ivec2 physical) {
		Size_t size;
		GLFWvidmode mode{ modes.Get() };
		if (setting::Setting::stt.FullScreen())
			mode = modes.Back();
		size.floating = size.integer = { mode.width, mode.height };
		size.center = { size.integer.x >> 1, size.integer.y >> 1 };
		size.pixel = { 1. / size.integer.x, 1. / size.integer.y };
		return size;
	}
}