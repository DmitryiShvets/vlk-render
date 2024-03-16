#pragma once
#include "device.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
namespace sve {

	class Model {
	public:

		struct Vertex
		{
			glm::vec2 pos;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> get_binding_description();
			static std::vector<VkVertexInputAttributeDescription> get_attribute_description();
		};

		Model(Device& device, const std::vector<Vertex>& vertices);
		~Model();

		void bind(VkCommandBuffer cmb_buff);
		void draw(VkCommandBuffer cmb_buff);

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
	private:
		void create_vertex_buffer(const std::vector<Vertex>& vertices);

		Device& m_device;
		VkBuffer vertex_buffer;
		VkDeviceMemory vertex_memory;
		uint32_t vertex_count;
	};

}