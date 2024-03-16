#include "main_window.h"
#include <stdexcept>
sve::MainWindow::MainWindow(int width, int height, std::string app_name) : width(width), height(height), window_name(app_name)
{
	init();
}

sve::MainWindow::~MainWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void sve::MainWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

inline void sve::MainWindow::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, resized_window_callback);
}

void sve::MainWindow::resized_window_callback(GLFWwindow* window, int width, int height)
{
	auto main_window = reinterpret_cast<MainWindow*> (glfwGetWindowUserPointer(window));
	main_window->framebuffer_resized = true;
	main_window->width = width;
	main_window->height = height;
}
