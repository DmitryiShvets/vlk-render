#pragma once
#include "main_window.h"
#include "pipeline.h"

namespace sve {
	class TestApp {
	public:

		void run();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
	private:
		MainWindow main_window{ WIDTH,HEIGHT,"Test app" };
		Device m_device{ main_window };
		PipeLine m_pipeline{ m_device,PipeLine::get_default_config(WIDTH,HEIGHT), "../../../../test2/res/shaders/vert.spv","../../../../test2/res/shaders/frag.spv" };
	};

}