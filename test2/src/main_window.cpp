#include "main_window.h"
#include <stdexcept>
sve::MainWindow::MainWindow(int width, int height, std::string app_name) : width(width), height(height),window_name(app_name)
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
}
