#include "frame.h"
#include "setting.h"

using Fr = frame::Frame;
Fr::Frame() :
	monitor{ std::invoke([]() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		return glfwGetPrimaryMonitor();
		}) },
	size{ std::invoke([&]() {
		Size size;
		size.physical.x = 169; size.physical.y = 127; //glfwGetMonitorPhysicalSize(monitor, &size.physical.x, &size.physical.y);
		auto mode{ glfwGetVideoMode(monitor) };
		size.integer.x = 640; // mode->width;
		size.integer.y = 480; // mode->height;
		size.floating = size.integer;
		constexpr int mm_in_inch = 254;
		size.dpi.x = size.integer.x * mm_in_inch / (size.physical.x * 10);
		size.dpi.y = size.integer.y * mm_in_inch / (size.physical.y * 10);
		return size;
		}) },
	window{ std::invoke([&]() {
		//auto window{ glfwCreateWindow(size.integer.x, size.integer.y, "The Rose and the Worm", NULL, NULL) };
		auto window{ glfwCreateWindow(640, 480, "TEST", NULL, NULL) };
		//glfwSetWindowMonitor(window, monitor, 0, 0, size.integer.x, size.integer.y, GLFW_DONT_CARE);
		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		return window;
		}) }
{
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, CursorPosCallBack);
	glfwSetMouseButtonCallback(window, MouseCallBack);
}
Fr::~Frame() {
	glfwTerminate();
}
/*
const Fr& Fr::operator<< (const graphics::Graphics& grph) const {
	grph.BindBuffer().BindTexture().BindShader().BindUniform();
	if (grph.Element::Size()) {
		glDrawElements(grph.Type(), grph.Element::Size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(grph.Type(), 0, grph.Array::Size());
	}
	return *this;
}
*/
const Fr& Fr::Swap() const {
	glfwSwapBuffers(window);
	return *this;
}
void Fr::CursorPosCallBack(GLFWwindow* window, double xpos, double ypos) {
	Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
	frame->cursor = glm::vec2{ xpos, ypos };
	frame->CallBack();
}
void Fr::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
	Frame* frame{ static_cast<Frame*>(glfwGetWindowUserPointer(window)) };
	frame->mouse = { button, action, mods };
	frame->CallBack();
	/*
		PostQuitMessage(0);
	*/
}