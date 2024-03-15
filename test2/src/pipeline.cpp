#include "pipeline.h"
#include <fstream>
#include <iostream>
sve::PipeLine::PipeLine(Device& device, const PipeLineConfig& config, const std::string& vert_shader_filepath, const std::string& frag_shader_filepath)
	: device(device), vert_shader(vert_shader_filepath), frag_shader(frag_shader_filepath)
{
	create_graphics_pipeline(config);
}

sve::PipeLine::~PipeLine()
{
}

sve::PipeLineConfig sve::PipeLine::get_default_config(int w, int h)
{
	return PipeLineConfig{};
}

std::vector<char> sve::PipeLine::read_file(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void sve::PipeLine::create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());


	if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shader_module) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}

void sve::PipeLine::create_graphics_pipeline(const PipeLineConfig& config)
{
	auto vert_code = read_file(vert_shader);
	auto frag_code = read_file(frag_shader);

	std::cout << "vert_shader size " << vert_code.size() << std::endl;
	std::cout << "frag_shader size " << frag_code.size() << std::endl;
}

