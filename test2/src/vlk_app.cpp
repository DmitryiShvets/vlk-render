#include "vlk_app.h"
#include <stdexcept>
#include <array>
void sve::TestApp::run() {
	while (!main_window.is_closing()) {
		glfwPollEvents();
		draw_frame();
	}
	vkDeviceWaitIdle(m_device.device());
}

sve::TestApp::TestApp()
{
	create_pipline_layout();
	create_pipline();
	create_command_buffer();
}

sve::TestApp::~TestApp()
{
	vkDestroyPipelineLayout(m_device.device(), m_pipeline_layout, nullptr);
}

void sve::TestApp::draw_frame()
{
	uint32_t image_index;
	auto result = m_swap_chain.acquireNextImage(&image_index);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	result = m_swap_chain.submitCommandBuffers(&m_command_buffers[image_index], &image_index);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void sve::TestApp::create_pipline_layout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional


	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void sve::TestApp::create_pipline()
{
	auto pipeline_config = PipeLine::get_default_config(m_swap_chain.width(), m_swap_chain.height());
	pipeline_config.renderPass = m_swap_chain.getRenderPass();
	pipeline_config.pipelineLayout = m_pipeline_layout;
	m_pipeline = std::make_unique<PipeLine>(m_device, pipeline_config, "../../../../test2/res/shaders/vert.spv", "../../../../test2/res/shaders/frag.spv");
}


void sve::TestApp::create_command_buffer()
{
	m_command_buffers.resize(m_swap_chain.imageCount());

	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_device.getCommandPool();
	alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command buffers!");
	}

	for (int i = 0; i < m_command_buffers.size(); i++)
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_command_buffers[i], &begin_info) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffers!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_swap_chain.getRenderPass();
		renderPassInfo.framebuffer = m_swap_chain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swap_chain.getSwapChainExtent();

		std::array<VkClearValue, 2> clear_values = {};
		clear_values[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clear_values[1].depthStencil = { 1.0f,0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
		renderPassInfo.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(m_command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		m_pipeline->bind_buffer(m_command_buffers[i]);
		vkCmdDraw(m_command_buffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_command_buffers[i]);

		if (vkEndCommandBuffer(m_command_buffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
