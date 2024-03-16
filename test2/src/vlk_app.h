#pragma once
#include "main_window.h"
#include "pipeline.h"
#include "swap_chain.h"
#include "model.h"
#include <memory>
namespace sve {
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
		void load_models();
		void create_pipline_layout();
		void create_pipline();
		void create_command_buffer();
		void record_command_buffer(int image_index);
		void free_command_buffer();
		void recreate_swapchain();
		MainWindow main_window{ WIDTH,HEIGHT,"Test app" };
		Device m_device{ main_window };
		//SwapChain m_swap_chain{ m_device,main_window.get_extent() };
		std::unique_ptr<SwapChain> m_swapchain;
		//PipeLine m_pipeline{ m_device,PipeLine::get_default_config(WIDTH,HEIGHT), "../../../../test2/res/shaders/vert.spv","../../../../test2/res/shaders/frag.spv" };
		std::unique_ptr<PipeLine> m_pipeline;
		std::unique_ptr<Model> m_model;
		VkPipelineLayout m_pipeline_layout;
		std::vector<VkCommandBuffer> m_command_buffers;
	};

}