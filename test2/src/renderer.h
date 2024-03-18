#pragma once

#include "main_window.h"
#include "swap_chain.h"
#include <memory>
#include <cassert>
namespace sve {

	class Renderer {
	public:

		Renderer(MainWindow& window, Device& device);
		~Renderer();
		VkRenderPass get_swapchain_renderpass() const { return m_swapchain->getRenderPass(); };
		float get_aspectratio() const { return m_swapchain->extentAspectRatio(); };
		bool is_frame_in_progress() const { return is_frame_started; };
		VkCommandBuffer get_cur_command_buffer() const {
			assert(is_frame_started && "Cannot get command when frame in progress!");
			return m_command_buffers[cur_frame_index];
		};

		VkCommandBuffer start_frame();
		void end_frame();
		void strart_swapchain_renderpass(VkCommandBuffer cmd_buff);
		void end_swapchain_renderpass(VkCommandBuffer cmd_buff);
		int get_frame_index()const {
			assert(is_frame_started && "Cannot get frame_index when frame isn't in progress!");
			return cur_frame_index;
		}
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
	private:

		void create_command_buffer();
		void free_command_buffer();
		void recreate_swapchain();

		MainWindow& m_window;
		Device& m_device;
		std::unique_ptr<SwapChain> m_swapchain;
		std::vector<VkCommandBuffer> m_command_buffers;

		uint32_t cur_image_index;
		int cur_frame_index = 0;
		bool is_frame_started = false;
	};

}