set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/imgui)

# ------ Core -------
add_library(im-core 
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
)
target_include_directories(im-core 
    PUBLIC
    $<BUILD_INTERFACE:${IMGUI_PATH}>
    $<INSTALL_INTERFACE:imgui.h>
    $<INSTALL_INTERFACE:imgui_internal.h>
)


# ------ GLFW -------
add_library(im-glfw ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp)
target_include_directories(im-glfw
    PUBLIC
    $<BUILD_INTERFACE:${IMGUI_PATH}/backends>
    $<INSTALL_INTERFACE:imgui_impl_glfw.h>
)
target_link_libraries(im-glfw
    im-core
    glfw
)

# ------ vulkan -------
add_library(im-vulkan ${IMGUI_PATH}/backends/imgui_impl_vulkan.cpp)

target_include_directories(im-vulkan
    PUBLIC
    $<BUILD_INTERFACE:${IMGUI_PATH}/backends>
    $<INSTALL_INTERFACE:imgui_impl_vulkan.h>
    PRIVATE ${IMGUI_PATH}
)
target_link_libraries(im-vulkan
   Vulkan::Vulkan
)

