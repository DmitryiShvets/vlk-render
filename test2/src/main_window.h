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
		bool is_window_resized() { return framebuffer_resized; };
		void reset_window_resize_flag() { framebuffer_resized = false; };
		MainWindow(const MainWindow&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
		bool is_closing() { return glfwWindowShouldClose(m_window); }
		VkExtent2D get_extent() { return VkExtent2D{ width,height }; }
		GLFWwindow* get_window_decrtiptor() const { return m_window; }
	private:

		void init();
		static void resized_window_callback(GLFWwindow* window, int widtg, int height);
		GLFWwindow* m_window;
		std::string window_name;
		uint32_t width;
		uint32_t height;
		bool framebuffer_resized = false;

	};
}