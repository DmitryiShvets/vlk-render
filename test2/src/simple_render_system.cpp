#include "simple_render_system.h"

#include <stdexcept>
#include <cassert>

struct PushConstantData
{
	glm::mat2 transform{ 1.0f };
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};


sve::SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass render_pass)
	:m_device(device)
{
	create_pipline_layout();
	create_pipline(render_pass);
}

sve::SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_device.device(), m_pipeline_layout, nullptr);
}


void sve::SimpleRenderSystem::create_pipline_layout()
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

void sve::SimpleRenderSystem::create_pipline(VkRenderPass render_pass)
{
	//assert(m_swapchain != nullptr && "Cannot create pipeline before swap chain");
	assert(m_pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipeline_config{};
	PipeLine::set_default_config(pipeline_config);
	pipeline_config.renderPass = render_pass;
	pipeline_config.pipelineLayout = m_pipeline_layout;

	m_pipeline = std::make_unique<PipeLine>(m_device,
		pipeline_config,
		"../../../../test2/res/shaders/v_test.vert.spv",
		"../../../../test2/res/shaders/f_test.frag.spv");
}



void sve::SimpleRenderSystem::render_gameobjects(VkCommandBuffer cmb_buff, std::vector<GameObject>& objects)
{
	m_pipeline->bind_buffer(cmb_buff);

	for (auto& obj : objects) {
		PushConstantData push_data{};
		push_data.offset = obj.transform.translation;
		push_data.color = obj.color;
		push_data.transform = obj.transform.mat2();

		vkCmdPushConstants(
			cmb_buff,
			m_pipeline_layout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PushConstantData),
			&push_data);

		obj.model->bind(cmb_buff);
		obj.model->draw(cmb_buff);
	}
}
