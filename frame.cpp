#include "frame.h"

frame::Modes_t::Modes_t(GLFWmonitor* const monitor) :
	Modes_t{ std::invoke([&]() {
		int count;
		auto mode{ glfwGetVideoModes(monitor, &count) };
		return Modes_t{ mode, count };
		}) }
{}
frame::Size_t::Size_t(const GLFWvidmode mode) :
	Size_t{ mode.width, mode.height }
{}
frame::Size_t::Size_t(const int width, const int height) :
	floating{ (float)width, (float)height, 0, 0 },
	pixel{ 1. / width, 1. / height, 0, 0 },
	integer{ width, height, 0, 0 },
	center{ (width + 1) >> 1, (height + 1) >> 1, 0, 0 }
{}
void frame::Size_t::operator= (const Size_t& other) {
	floating = other.floating;
	pixel = other.pixel;
	integer = other.integer;
	center = other.center;
}

frame::frame() :
	frame{ std::invoke([]() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		return glfwGetPrimaryMonitor();
		}) }
{}
frame::frame(GLFWmonitor* monitor) :
	frame{ monitor, std::invoke([&]() {
		int count;
		auto mode{ glfwGetVideoModes(monitor, &count) };
		return Modes_t{ mode, count };
		}) }
{}
frame::frame(GLFWmonitor* const monitor, const Modes_t modes) :
	frame{ modes, std::invoke([&]() {
		int X,Y;
		glfwGetMonitorPhysicalSize(monitor, &X, &Y);
		return glm::ivec2{ X,Y };
		}) }
{}
frame::frame(const Modes_t modes, const glm::ivec2 physical) :
	frame{ modes, physical, std::invoke([&]() {
		glm::vec2 dpi;
		auto& mode{ modes.ptr[modes.count - 1] };
		constexpr int mm_in_inch = 254;
		dpi.x = mode.width * mm_in_inch / (physical.x * 10);
		dpi.y = mode.height * mm_in_inch / (physical.y * 10);
		return dpi;
		})
}
{};
frame::frame(const Modes_t modes_, const glm::ivec2 physical_, const glm::vec2 dpi_) :
	modes{ modes_ },
	size{ modes.Get() },
	dpi{ dpi_ },
	window{ glfwCreateWindow(size.integer.x, size.integer.y, "The Rose and the Worm", NULL, NULL) },
	uniform{ std::invoke([&]() {
		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		constexpr unsigned index = 0;
		unsigned uniform = 0;
		glGenBuffers(1, &uniform);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Size_t), &size, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, uniform);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return uniform;
		}) }
{
	glfwSetWindowSize(window, size.integer.x, size.integer.y);

	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, CursorPosCallBack);
	glfwSetMouseButtonCallback(window, MouseCallBack);

	glClearColor(0., 0., 0., 1.0f);
	glfwSwapBuffers(window);
}
frame::~frame() {
	glfwTerminate();
}

void frame::CursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
	frame* frm{ static_cast<frame*>(glfwGetWindowUserPointer(window)) };
	(*frm.*frm->call_back)(
		glm::ivec2{ xpos - frm->size.center.x, frm->size.center.y - ypos }, std::nullopt);
	(*frm.*frm->call_paint)();
	glfwSwapBuffers(window);
}
void frame::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
	frame* frm{ static_cast<frame*>(glfwGetWindowUserPointer(window)) };
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	(*frm.*frm->call_back)(glm::ivec2{ xpos - frm->size.center.x, frm->size.center.y - ypos },
		std::tuple{ button, action, mods });
	(*frm.*frm->call_paint)();
	glfwSwapBuffers(window);
}
void frame::UpMode() {
	frm.modes.Up();
	frm.size = Size_t{ frm.modes.Get() };
	glfwSetWindowSize(frm.window, frm.size.integer.x, frm.size.integer.y);
	glViewport(0, 0, frm.size.integer.x, frm.size.integer.y);

	glBindBuffer(GL_UNIFORM_BUFFER, frm.uniform);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Size_t), &frm.size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}