#include "model.h"
#include <cassert>
#include <cstring>
sve::Model::Model(Device& device, const std::vector<Vertex>& vertices) :m_device(device)
{
	create_vertex_buffer(vertices);
}

sve::Model::~Model()
{
	vkDestroyBuffer(m_device.device(), vertex_buffer, nullptr);
	vkFreeMemory(m_device.device(), vertex_memory, nullptr);
}

void sve::Model::bind(VkCommandBuffer cmb_buff)
{
	VkBuffer buffers[] = { vertex_buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmb_buff, 0, 1, buffers, offsets);
}

void sve::Model::draw(VkCommandBuffer cmb_buff)
{
	vkCmdDraw(cmb_buff, vertex_count, 1, 0, 0);
}

void sve::Model::create_vertex_buffer(const std::vector<Vertex>& vertices)
{
	vertex_count = static_cast<uint32_t> (vertices.size());
	assert(vertex_count >= 3 && "Vertex count must be at least 3!");

	VkDeviceSize buff_size = sizeof(vertices[0]) * vertex_count;

	m_device.createBuffer(
		buff_size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertex_buffer,
		vertex_memory
	);

	void* data;
	vkMapMemory(m_device.device(), vertex_memory, 0, buff_size, 0, &data);
	memcpy(data, vertices.data(), static_cast<uint32_t> (buff_size));
	vkUnmapMemory(m_device.device(), vertex_memory);
}

std::vector<VkVertexInputBindingDescription> sve::Model::Vertex::get_binding_description()
{
	std::vector<VkVertexInputBindingDescription> binding_decription(1);
	binding_decription[0].binding = 0;
	binding_decription[0].stride = sizeof(Vertex);
	binding_decription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return binding_decription;
}

std::vector<VkVertexInputAttributeDescription> sve::Model::Vertex::get_attribute_description()
{
	std::vector<VkVertexInputAttributeDescription> attrib_decription(2);
	attrib_decription[0].binding = 0;
	attrib_decription[0].location = 0;
	attrib_decription[0].offset = offsetof(Vertex, position);
	attrib_decription[0].format = VK_FORMAT_R32G32B32_SFLOAT;

	attrib_decription[1].binding = 0;
	attrib_decription[1].location = 1;
	attrib_decription[1].offset = offsetof(Vertex, color);
	attrib_decription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	return attrib_decription;
}
