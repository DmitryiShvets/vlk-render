set(PROJ_NAME SIMPLE_APP)
set(PROJ_SRC 
				"main.cpp"
			 "src/application.h" "src/application.cpp")

add_executable (${PROJ_NAME} ${PROJ_SRC})

target_include_directories(${PROJ_NAME} PRIVATE src)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJ_NAME} PRIVATE Vulkan::Vulkan glfw glm)

