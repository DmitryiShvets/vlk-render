#pragma once

#include "pipeline.h"
#include "game_object.h"

namespace sve {

	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(Device& device,VkRenderPass render_pass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void render_gameobjects(VkCommandBuffer cmb_buff, std::vector<GameObject>& objects);

	private:
		void create_pipline_layout();
		void create_pipline(VkRenderPass render_pass);

		Device& m_device;
	
		std::unique_ptr<PipeLine> m_pipeline;
		VkPipelineLayout m_pipeline_layout;
	};

}