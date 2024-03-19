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
			 "src/swap_chain.h"
			 "src/swap_chain.cpp"
			 "src/model.h"
			 "src/model.cpp"
			 "src/game_object.h"
			 "src/game_object.cpp"
			 "src/renderer.h"
 "src/renderer.cpp" 
 "src/simple_render_system.h"
 "src/simple_render_system.cpp"
 "src/stb_image.h"
 "src/texture.h"
 "src/texture.cpp"
 "src/camera.h" 
 "src/camera.cpp" 
 "src/user_input_controller.h"
 "src/user_input_controller.cpp"
 "src/tiny_obj_loader.h"
 "src/utils.h"
 "src/buffer.h"
 "src/buffer.cpp"
 "src/frame_info.h")

add_executable (${PROJ_NAME} ${PROJ_SRC} )

target_include_directories(${PROJ_NAME} PRIVATE src)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJ_NAME} PRIVATE Vulkan::Vulkan glfw glm)
