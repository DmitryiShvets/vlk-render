#include "vlk_app.h"
#include "simple_render_system.h"
#include <glm/gtc/constants.hpp>

void sve::TestApp::run() {
	SimpleRenderSystem render_system{ m_device,m_renderer.get_swapchain_renderpass() };

	while (!main_window.is_closing()) {
		glfwPollEvents();

		if (auto cmb_buff = m_renderer.start_frame()) {
			m_renderer.strart_swapchain_renderpass(cmb_buff);
			render_system.render_gameobjects(cmb_buff, m_objects);
			m_renderer.end_swapchain_renderpass(cmb_buff);
			m_renderer.end_frame();
		}
	}
	vkDeviceWaitIdle(m_device.device());
}

sve::TestApp::TestApp()
{
	load_gameobjects();
}

sve::TestApp::~TestApp()
{
}


void sve::TestApp::load_gameobjects()
{
	std::vector<Model::Vertex> vertices{
	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	auto m_model = std::make_shared<Model>(m_device, vertices);

	auto triangle = GameObject::create_gameobject();
	triangle.model = m_model;
	triangle.color = { 0.1f, 0.8f, 0.1f };
	triangle.transform.translation.x = 0.2f;
	triangle.transform.scale.y = 0.5f;
	triangle.transform.scale.x = 2.5f;
	triangle.transform.rotation = 0.25f * glm::two_pi<float>();

	m_objects.push_back(std::move(triangle));
}
