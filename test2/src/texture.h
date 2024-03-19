#pragma once

#include <stdexcept>
#include <device.h>
namespace sve {

	class Texture {
	public:
		Texture(Device& device, const std::string& filepath);
		~Texture();

		VkDescriptorImageInfo imageInfo{};

	private:
		Device& m_device;

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		void createTextureSampler();

		void createTextureImageView();

		void createTextureImage(const std::string& filepath);
	};

}