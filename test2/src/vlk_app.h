#pragma once
#include "main_window.h"
#include "renderer.h"
#include "game_object.h"
#include <memory>
namespace sve {

	class TestApp {
	public:

		void run();
		TestApp();
		~TestApp();
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		TestApp(const TestApp&) = delete;
		TestApp& operator=(const TestApp&) = delete;

	private:
		void load_gameobjects();

		MainWindow main_window{ WIDTH,HEIGHT,"Test app" };
		Device m_device{ main_window };
		Renderer m_renderer{ main_window,m_device };

		std::vector<GameObject> m_objects;
	};

}