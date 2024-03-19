#include "vlk_app.h"
#include "simple_render_system.h"
#include "camera.h"
#include "user_input_controller.h"
#include "buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

namespace sve {
	struct GlobalUBO
	{
		glm::mat4 proj_matrix{ 1.0f };
		glm::vec3 light_direction = glm::normalize(glm::vec3{ -1.0f,-3.0f,-1.0f });
	};
}

void sve::TestApp::run() {

	std::vector<std::unique_ptr<DataBuffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < uboBuffers.size(); i++) {
		uboBuffers[i] = std::make_unique<DataBuffer>(
			m_device,
			sizeof(GlobalUBO),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		uboBuffers[i]->map();
	}

	auto g_set_layout = DescriptorSetLayout::Builder(m_device)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.build();

	std::vector<VkDescriptorSet> g_descriptor_sets(SwapChain::MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < g_descriptor_sets.size(); i++) {
		auto bufferInfo = uboBuffers[i]->descriptorInfo();
		DescriptorWriter(*g_set_layout, *g_descriptor_pool)
			.writeBuffer(0, &bufferInfo)
			.build(g_descriptor_sets[i]);
	}

	SimpleRenderSystem render_system{ m_device,m_renderer.get_swapchain_renderpass(),g_set_layout->getDescriptorSetLayout()};
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
		float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - curr_time).count();
		curr_time = new_time;

		camera_controller.moveInPlaneXZ(main_window.get_window_decrtiptor(), frame_time, camera_object);
		camera.setViewYXZ(camera_object.transform.translation, camera_object.transform.rotation);

		aspect = m_renderer.get_aspectratio();
		//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
		camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

		if (auto cmb_buff = m_renderer.start_frame()) {
		
			int frame_index = m_renderer.get_frame_index();
			FrameInfo frame_info{
				frame_index,
				frame_time,
				cmb_buff,
				camera,
				g_descriptor_sets[frame_index]
			};

			GlobalUBO ubo{};
			ubo.proj_matrix = camera.getProjection() * camera.getView();
			uboBuffers[frame_index]->writeToBuffer(&ubo);
			uboBuffers[frame_index]->flush();

			m_renderer.strart_swapchain_renderpass(cmb_buff);
			render_system.render_gameobjects(frame_info, m_objects);
			m_renderer.end_swapchain_renderpass(cmb_buff);
			m_renderer.end_frame();
		}
	}
	vkDeviceWaitIdle(m_device.device());
}

sve::TestApp::TestApp()
{
	g_descriptor_pool =
		DescriptorPool::Builder(m_device)
		.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();

	load_gameobjects();


}

sve::TestApp::~TestApp()
{
}


void sve::TestApp::load_gameobjects()
{
	///--------------2 квадрата-------------
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


	///--------------куб-------------
	//std::shared_ptr<Model> lveModel = createCubeModel(m_device, { .0f, .0f, .0f });
	//auto cube = GameObject::create_gameobject();
	//cube.model = lveModel;
	//cube.transform.translation = { .0f, .0f, 2.5f };
	//cube.transform.scale = { .5f, .5f, .5f };
	//m_objects.push_back(std::move(cube));

	///--------------ваза-------------
	//std::shared_ptr<Model> lveModel = Model::create_model_fromfile(m_device, "../../../../test2/res/models/colored_cube.obj");
	std::shared_ptr<Model> flat_mesh = Model::create_model_fromfile(m_device, "../../../../test2/res/models/flat_vase.obj");
	auto flat = GameObject::create_gameobject();
	flat.model = flat_mesh;
	flat.transform.translation = { -.5f, .5f, 2.5f };
	flat.transform.scale = { 3.0f,2.0f,3.0f };
	m_objects.push_back(std::move(flat));

	std::shared_ptr<Model> smooth_mesh = Model::create_model_fromfile(m_device, "../../../../test2/res/models/smooth_vase.obj");
	auto smooth = GameObject::create_gameobject();
	smooth.model = smooth_mesh;
	smooth.transform.translation = { .5f, .5f, 2.5f };
	smooth.transform.scale = { 3.0f,2.0f,3.0f };
	m_objects.push_back(std::move(smooth));

}
