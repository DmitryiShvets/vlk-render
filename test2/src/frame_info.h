#pragma once
#include "camera.h"

// lib
#include <vulkan/vulkan.h>

namespace sve {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet descriptor_set;
	};
}  // 