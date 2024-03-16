#include "vlk_app.h"
#include <stdexcept>
#include <array>
#include <cassert>


void sve::TestApp::run() {
	while (!main_window.is_closing()) {
		glfwPollEvents();
		draw_frame();
	}
	vkDeviceWaitIdle(m_device.device());
}

sve::TestApp::TestApp()
{
	load_models();
	create_pipline_layout();
	recreate_swapchain();
	create_command_buffer();
}

sve::TestApp::~TestApp()
{
	vkDestroyPipelineLayout(m_device.device(), m_pipeline_layout, nullptr);
}

void sve::TestApp::draw_frame()
{
	uint32_t image_index;
	auto result = m_swapchain->acquireNextImage(&image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreate_swapchain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	record_command_buffer(image_index);
	result = m_swapchain->submitCommandBuffers(&m_command_buffers[image_index], &image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || main_window.is_window_resized()) {
		main_window.reset_window_resize_flag();
		recreate_swapchain();
		return;
	}

	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void sve::TestApp::load_models()
{
	std::vector<Model::Vertex> vertices{
	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	m_model = std::make_unique<Model>(m_device, vertices);
}

void sve::TestApp::create_pipline_layout()
{
	VkPushConstantRange push_constant_range{};
	push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	push_constant_range.offset = 0;
	push_constant_range.size = sizeof(PushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
	pipelineLayoutInfo.pPushConstantRanges = &push_constant_range; // Optional


	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void sve::TestApp::create_pipline()
{
	assert(m_swapchain != nullptr && "Cannot create pipeline before swap chain");
	assert(m_pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipeline_config{};
	PipeLine::set_default_config(pipeline_config);
	pipeline_config.renderPass = m_swapchain->getRenderPass();
	pipeline_config.pipelineLayout = m_pipeline_layout;
	m_pipeline = std::make_unique<PipeLine>(m_device,
		pipeline_config,
		"../../../../test2/res/shaders/v_test.vert.spv",
		"../../../../test2/res/shaders/f_test.frag.spv");
}

void sve::TestApp::recreate_swapchain()
{
	auto extent = main_window.get_extent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = main_window.get_extent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device.device());
	if (m_swapchain == nullptr) {
		m_swapchain = std::make_unique<SwapChain>(m_device, extent);
	}
	else {
		m_swapchain = std::make_unique<SwapChain>(m_device, extent, std::move(m_swapchain));
		if (m_swapchain->imageCount() != m_command_buffers.size()) {
			free_command_buffer();
			create_command_buffer();
		}
	}
	create_pipline();
}

void sve::TestApp::create_command_buffer()
{
	m_command_buffers.resize(m_swapchain->imageCount());

	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_device.getCommandPool();
	alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command buffers!");
	}

}

void sve::TestApp::free_command_buffer()
{
	vkFreeCommandBuffers(
		m_device.device(),
		m_device.getCommandPool(),
		static_cast<uint32_t>(m_command_buffers.size()),
		m_command_buffers.data());

	m_command_buffers.clear();
}



void sve::TestApp::record_command_buffer(int image_index)
{
	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(m_command_buffers[image_index], &begin_info) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffers!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_swapchain->getRenderPass();
	renderPassInfo.framebuffer = m_swapchain->getFrameBuffer(image_index);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_swapchain->getSwapChainExtent();

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clear_values[1].depthStencil = { 1.0f,0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
	renderPassInfo.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(m_command_buffers[image_index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_swapchain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_swapchain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, m_swapchain->getSwapChainExtent() };
	vkCmdSetViewport(m_command_buffers[image_index], 0, 1, &viewport);
	vkCmdSetScissor(m_command_buffers[image_index], 0, 1, &scissor);

	m_pipeline->bind_buffer(m_command_buffers[image_index]);
	m_model->bind(m_command_buffers[image_index]);

	for (int j = 0; j < 4; j++)
	{
		PushConstantData constant{};
		constant.offset = { 0.0f, -0.4f + j * 0.25f };
		constant.color = { 0.0f, 0.0f, 0.2f + j * 0.2f };

		vkCmdPushConstants(
			m_command_buffers[image_index],
			m_pipeline_layout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PushConstantData),
			&constant);

		m_model->draw(m_command_buffers[image_index]);
	}


	vkCmdEndRenderPass(m_command_buffers[image_index]);

	if (vkEndCommandBuffer(m_command_buffers[image_index]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}
