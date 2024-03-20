#include "renderer.h"

#include <stdexcept>
#include <array>
#include <cassert>


sve::Renderer::Renderer(MainWindow& window, Device& device)
	: m_window(window), m_device(device)
{
	recreate_swapchain();
	create_command_buffer();
}

sve::Renderer::~Renderer()
{
	free_command_buffer();
}

VkCommandBuffer sve::Renderer::start_frame()
{
	assert(!is_frame_started && "Can't call begin_frame while already in progress!");

	auto result = m_swapchain->acquireNextImage(&cur_image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreate_swapchain();
		return nullptr;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	is_frame_started = true;

	auto cmd_buff = get_cur_command_buffer();

	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(cmd_buff, &begin_info) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffers!");
	}

	return cmd_buff;
}

void sve::Renderer::end_frame()
{
	assert(is_frame_started && "Can't call end_frame while frame isn't in progress!");
	auto cmd_buff = get_cur_command_buffer();

	if (vkEndCommandBuffer(cmd_buff) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	auto result = m_swapchain->submitCommandBuffers(&cmd_buff, &cur_image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.is_window_resized()) {
		m_window.reset_window_resize_flag();
		recreate_swapchain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	is_frame_started = false;

	cur_frame_index = (cur_frame_index + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void sve::Renderer::strart_swapchain_renderpass(VkCommandBuffer cmd_buff)
{
	assert(is_frame_started && "Can't call strart_swapchain_renderpass while frame isn't in progress!");
	assert(cmd_buff == get_cur_command_buffer() && "Can't strart_swapchain_renderpass on command buffer from a different frame!");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_swapchain->getRenderPass();
	renderPassInfo.framebuffer = m_swapchain->getFrameBuffer(cur_image_index);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_swapchain->getSwapChainExtent();

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clear_values[1].depthStencil = { 1.0f,0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
	renderPassInfo.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(cmd_buff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_swapchain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_swapchain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, m_swapchain->getSwapChainExtent() };
	vkCmdSetViewport(cmd_buff, 0, 1, &viewport);
	vkCmdSetScissor(cmd_buff, 0, 1, &scissor);
}

void sve::Renderer::end_swapchain_renderpass(VkCommandBuffer cmd_buff)
{
	assert(is_frame_started && "Can't call end_swapchain_renderpass while frame isn't in progress!");
	assert(cmd_buff == get_cur_command_buffer() && "Can't end_swapchain_renderpass on command buffer from a different frame!");

	vkCmdEndRenderPass(cmd_buff);
}


void sve::Renderer::recreate_swapchain()
{
	auto extent = m_window.get_extent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_window.get_extent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device.device());
	if (m_swapchain == nullptr) {
		m_swapchain = std::make_unique<SwapChain>(m_device, extent);
	}
	else {
		std::shared_ptr<SwapChain> old_swapchain = std::move(m_swapchain);
		m_swapchain = std::make_unique<SwapChain>(m_device, extent, old_swapchain);

		if (!old_swapchain->comapare_formats(*m_swapchain.get())) {
			throw std::runtime_error("Swap chain image(depth) format has changed!");
		}
	}

	//todo
}



void sve::Renderer::create_command_buffer()
{
	m_command_buffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_device.getCommandPool();
	alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command buffers!");
	}
}

void sve::Renderer::free_command_buffer()
{
	vkFreeCommandBuffers(
		m_device.device(),
		m_device.getCommandPool(),
		static_cast<uint32_t>(m_command_buffers.size()),
		m_command_buffers.data());

	m_command_buffers.clear();
}




