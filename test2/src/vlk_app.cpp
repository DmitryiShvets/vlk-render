#include "vlk_app.h"
#include <stdexcept>
void sve::TestApp::run() {
	while (!main_window.is_closing()) {
		glfwPollEvents();

	}
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
}
