#pragma once
#include "camera.h"
#include "pipeline.h"
#include "game_object.h"
#include "frame_info.h"

namespace sve {

	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(Device& device,VkRenderPass render_pass, VkDescriptorSetLayout descriptor_layout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void render_gameobjects(FrameInfo& frame_info, std::vector<GameObject>& objects);

	private:
		void create_pipline_layout(VkDescriptorSetLayout descriptor_layout);
		void create_pipline(VkRenderPass render_pass);

		Device& m_device;
	
		std::unique_ptr<PipeLine> m_pipeline;
		VkPipelineLayout m_pipeline_layout;
	};

}