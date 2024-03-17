#pragma once
#include "main_window.h"
#include "pipeline.h"
#include "swap_chain.h"
#include "game_object.h"
#include <memory>
namespace sve {

	struct PushConstantData
	{
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};


	class TestApp {
	public:



		void run();
		TestApp();
		~TestApp();
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void draw_frame();

		TestApp(const TestApp&) = delete;
		TestApp& operator=(const TestApp&) = delete;
	private:
		void load_gameobjects();
		void create_pipline_layout();
		void create_pipline();
		void create_command_buffer();
		void record_command_buffer(int image_index);
		void free_command_buffer();
		void recreate_swapchain();
		void render_gameobject(VkCommandBuffer cmb_buff);
		MainWindow main_window{ WIDTH,HEIGHT,"Test app" };
		Device m_device{ main_window };
		std::unique_ptr<SwapChain> m_swapchain;
		std::unique_ptr<PipeLine> m_pipeline;
		
		VkPipelineLayout m_pipeline_layout;
		std::vector<VkCommandBuffer> m_command_buffers;
		std::vector<GameObject> m_objects;
	};

}