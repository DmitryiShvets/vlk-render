#include "model.h"
#include "utils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <iostream>

#include <unordered_map>
#include <cassert>
#include <cstring>



std::unique_ptr<sve::Model> sve::Model::create_model_fromfile(Device& device, const std::string& file)
{
	Builder builder{};
	builder.load_model(file);
	std::cout << "Vertex count: " << builder.vertices.size() << std::endl;
	return std::make_unique<sve::Model>(device, builder);
}

sve::Model::Model(Device& device, const Builder& builder) :m_device(device)
{
	create_vertex_buffer(builder.vertices);
	create_index_buffer(builder.indices);
}

sve::Model::~Model()
{
	vkDestroyBuffer(m_device.device(), vertex_buffer, nullptr);
	vkFreeMemory(m_device.device(), vertex_memory, nullptr);

	if (has_indexbuffer) {
		vkDestroyBuffer(m_device.device(), index_buffer, nullptr);
		vkFreeMemory(m_device.device(), index_memory, nullptr);
	}
}

void sve::Model::bind(VkCommandBuffer cmb_buff)
{
	VkBuffer buffers[] = { vertex_buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmb_buff, 0, 1, buffers, offsets);
	if (has_indexbuffer) {
		vkCmdBindIndexBuffer(cmb_buff, index_buffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void sve::Model::draw(VkCommandBuffer cmb_buff)
{
	if (has_indexbuffer) {
		vkCmdDrawIndexed(cmb_buff, index_count, 1, 0, 0, 0);
	}
	else {
		vkCmdDraw(cmb_buff, vertex_count, 1, 0, 0);
	}
}

void sve::Model::create_vertex_buffer(const std::vector<Vertex>& vertices)
{
	vertex_count = static_cast<uint32_t> (vertices.size());
	assert(vertex_count >= 3 && "Vertex count must be at least 3!");

	VkDeviceSize buff_size = sizeof(vertices[0]) * vertex_count;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.createBuffer(
		buff_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, buff_size, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(buff_size));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(
		buff_size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertex_buffer,
		vertex_memory
	);
	m_device.copyBuffer(stagingBuffer, vertex_buffer, buff_size);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

void sve::Model::create_index_buffer(const std::vector<uint32_t>& indices)
{
	index_count = static_cast<uint32_t> (indices.size());
	has_indexbuffer = index_count > 0;

	if (!has_indexbuffer) {
		return;
	}

	VkDeviceSize buff_size = sizeof(indices[0]) * index_count;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.createBuffer(
		buff_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, buff_size, 0, &data);
	memcpy(data, indices.data(), static_cast<size_t>(buff_size));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(
		buff_size,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		index_buffer,
		index_memory
	);
	m_device.copyBuffer(stagingBuffer, index_buffer, buff_size);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
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

void sve::Model::Builder::load_model(const std::string& file)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				auto colorIndex = 3 * index.vertex_index + 2;
				if (colorIndex < attrib.colors.size()) {
					vertex.color = {
						attrib.colors[colorIndex - 2],
						attrib.colors[colorIndex - 1],
						attrib.colors[colorIndex - 0],
					};
				}
				else {
					vertex.color = { 1.f, 1.f, 1.f };  // set default color
				}
			}

			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
