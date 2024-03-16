#pragma once
#include <string>
#include <vector>
#include "device.h"

namespace sve {

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class PipeLine {
	public:
		PipeLine(Device& device,const PipelineConfigInfo& config,const std::string& vert_shader_filepath, const std::string& frag_shader_filepath);
		~PipeLine();

		void bind_buffer(VkCommandBuffer buffer);
		PipeLine(const PipeLine&) = delete;
		PipeLine& operator=(const PipeLine&) = delete;
		static void set_default_config(PipelineConfigInfo& config);

	private:
		static std::vector<char> read_file(const std::string& filepath);
		void create_graphics_pipeline(const std::string& vert_shader_filepath, const std::string& frag_shader_filepath,const PipelineConfigInfo& config);
		void create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module);
		Device& m_device;
		VkPipeline m_graphics_pipeline;
		VkShaderModule m_vert_shader_module;
		VkShaderModule m_frag_shader_module;
	};

}