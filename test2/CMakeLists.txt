set(PROJ_NAME TEST_VLK_APP)
set(PROJ_SRC 
				"main.cpp"
			 "src/main_window.h"
			 "src/main_window.cpp"
			 "src/vlk_app.h" 
			 "src/vlk_app.cpp" 
			 "src/pipeline.h" 
			 "src/pipeline.cpp"
			 "src/device.h"
			 "src/device.cpp"
)

add_executable (${PROJ_NAME} ${PROJ_SRC})

target_include_directories(${PROJ_NAME} PRIVATE src)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJ_NAME} PRIVATE Vulkan::Vulkan glfw glm)

