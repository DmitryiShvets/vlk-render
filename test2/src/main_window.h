#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
namespace sve {

	class MainWindow {
	public:

		MainWindow(int width, int height, std::string app_name);
		~MainWindow();
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
		bool is_closing() { return glfwWindowShouldClose(m_window); }
		VkExtent2D get_extent() { return VkExtent2D{ width,height }; }
	private:

		void init();

		GLFWwindow* m_window;
		std::string window_name;
		const uint32_t width;
		const uint32_t height;
	};
}