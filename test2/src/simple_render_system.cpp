#include "simple_render_system.h"

#include <stdexcept>
#include <cassert>
#include <glm/gtc/constants.hpp>
struct PushConstantData
{
	glm::mat4 modelMatrix{ 1.0f };
	glm::mat4 normalMatrix{ 1.f };
};


sve::SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass render_pass, VkDescriptorSetLayout descriptor_layout)
	:m_device(device)
{
	create_pipline_layout(descriptor_layout);
	create_pipline(render_pass);
}

sve::SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_device.device(), m_pipeline_layout, nullptr);
}


void sve::SimpleRenderSystem::create_pipline_layout(VkDescriptorSetLayout descriptor_layout)
{
	VkPushConstantRange push_constant_range{};
	push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	push_constant_range.offset = 0;
	push_constant_range.size = sizeof(PushConstantData);


	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ descriptor_layout };


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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
		"../../../../test2/res/shaders/texture.vert.spv",
		"../../../../test2/res/shaders/texture.frag.spv");
}



void sve::SimpleRenderSystem::render_gameobjects(FrameInfo& frame_info, std::vector<GameObject>& objects)
{
	m_pipeline->bind_buffer(frame_info.commandBuffer);

	for (auto& obj : objects) {
		//obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
		//obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

		vkCmdBindDescriptorSets(
			frame_info.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_pipeline_layout,
			0,
			1,
			&frame_info.descriptor_set,
			0,
			nullptr);

		PushConstantData push_data{};

		push_data.modelMatrix = obj.transform.model_matrix();
		push_data.normalMatrix = obj.transform.normal_matrix();

		vkCmdPushConstants(
			frame_info.commandBuffer,
			m_pipeline_layout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PushConstantData),
			&push_data);

		obj.model->bind(frame_info.commandBuffer);
		obj.model->draw(frame_info.commandBuffer);
	}
}
