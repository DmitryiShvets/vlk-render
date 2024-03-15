#pragma once
#include <string>
#include <vector>
#include "device.h"

namespace sve {

	struct PipeLineConfig
	{

	};

	class PipeLine {
	public:
		PipeLine(Device& device,const PipeLineConfig& config,const std::string& vert_shader_filepath, const std::string& frag_shader_filepath);
		~PipeLine();

		PipeLine(const PipeLine&) = delete;
		PipeLine& operator=(const PipeLine&) = delete;
		static PipeLineConfig get_default_config(int w, int h);

	private:
		static std::vector<char> read_file(const std::string& filepath);
		void create_graphics_pipeline(const PipeLineConfig& config);
		void create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module);
		std::string vert_shader;
		std::string frag_shader;
		Device& device;
		VkPipeline graphics_pipeline;
		VkShaderModule vert_shader_module;
		VkShaderModule frag_shader_module;
	};

}