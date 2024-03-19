#pragma once
#include "device.h"
#include "buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>

namespace sve {

	class Model {
	public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal;
			glm::vec2 uv;

			static std::vector<VkVertexInputBindingDescription> get_binding_description();
			static std::vector<VkVertexInputAttributeDescription> get_attribute_description();

			bool operator==(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal &&
					uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void load_model(const std::string& file);	
		};

		static std::unique_ptr<Model> create_model_fromfile( Device& device,const std::string& file);

		Model(Device& device, const Builder& builder);
		~Model();

		void bind(VkCommandBuffer cmb_buff);
		void draw(VkCommandBuffer cmb_buff);

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
	private:
		void create_vertex_buffer(const std::vector<Vertex>& vertices);
		void create_index_buffer(const std::vector<uint32_t>& indices);

		Device& m_device;

		std::unique_ptr<DataBuffer> vertex_buffer;
		uint32_t vertex_count;

		std::unique_ptr<DataBuffer> index_buffer;
		uint32_t index_count;

		bool has_indexbuffer = false;
	};

}