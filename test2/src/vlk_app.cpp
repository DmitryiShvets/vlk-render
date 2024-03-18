#include "vlk_app.h"
#include "simple_render_system.h"
#include "camera.h"
#include "user_input_controller.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

void sve::TestApp::run() {
	SimpleRenderSystem render_system{ m_device,m_renderer.get_swapchain_renderpass() };
	Camera camera{};
	float aspect;
	//camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
	//camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

	auto camera_object = GameObject::create_gameobject();
	KeyboardMovementController camera_controller{};

	auto curr_time = std::chrono::high_resolution_clock::now();

	while (!main_window.is_closing()) {
		glfwPollEvents();

		auto new_time = std::chrono::high_resolution_clock::now();
		float duration_frame = std::chrono::duration<float, std::chrono::seconds::period>(new_time - curr_time).count();
		curr_time = new_time;

		camera_controller.moveInPlaneXZ(main_window.get_window_decrtiptor(), duration_frame, camera_object);
		camera.setViewYXZ(camera_object.transform.translation, camera_object.transform.rotation);

		aspect = m_renderer.get_aspectratio();
		//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
		camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

		if (auto cmb_buff = m_renderer.start_frame()) {
			m_renderer.strart_swapchain_renderpass(cmb_buff);
			render_system.render_gameobjects(cmb_buff, m_objects, camera);
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
	//std::vector<Model::Vertex> vertices{
	//{{-0.5f, -0.5f,0.0f}, {1.0f, 1.0f, 1.0f}},
	//{{-0.5f, 0.5f,0.0f}, {0.0f, 1.0f, 0.0f}},
	//{{0.5f, 0.5f,0.0f}, {0.0f, 1.0f, 0.0f}},
	//{{0.5f, 0.5f,0.0f}, {0.0f, 1.0f, 0.0f}},
	//{{0.5f, -0.5f,0.0f}, {0.0f, 0.0f, 1.0f}},
	//{ { -0.5f, -0.5f,0.0f }, {1.0f, 1.0f, 1.0f} }
	//};

	//auto m_model = std::make_shared<Model>(m_device, vertices);

	//auto triangle = GameObject::create_gameobject();
	//triangle.model = m_model;
	//triangle.color = { 0.0f, 0.0f, 0.0f };
	//triangle.transform.translation.x = 0.5f;

	//auto triangle1 = GameObject::create_gameobject();
	//triangle1.transform.translation.x = -0.5f;
	//triangle1.model = m_model;
	//triangle1.color = { 1.0f, 1.0f, 1.0f };

	//m_objects.push_back(std::move(triangle));
	//m_objects.push_back(std::move(triangle1));

	std::shared_ptr<Model> lveModel = createCubeModel(m_device, { .0f, .0f, .0f });
	auto cube = GameObject::create_gameobject();
	cube.model = lveModel;
	cube.transform.translation = { .0f, .0f, 2.5f };
	cube.transform.scale = { .5f, .5f, .5f };
	m_objects.push_back(std::move(cube));
}
