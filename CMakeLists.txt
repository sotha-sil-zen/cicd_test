#using sudo cmake . and sudo make
cmake_minimum_required(VERSION 3.16)

project(main)
# Assimp Part
set(ASSIMP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp)
set(ASSIMP_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/assimp)
add_subdirectory(${ASSIMP_SOURCE_DIR})

# 将 Assimp 安装到指定目录
install(TARGETS assimp
        EXPORT AssimpTargets
        ARCHIVE DESTINATION ${ASSIMP_INSTALL_DIR}/lib
        LIBRARY DESTINATION ${ASSIMP_INSTALL_DIR}/lib
        RUNTIME DESTINATION ${ASSIMP_INSTALL_DIR}/bin
)

# 导出 Assimp 的 CMake 配置文件
install(EXPORT AssimpTargets
        FILE AssimpTargets.cmake
        DESTINATION ${ASSIMP_INSTALL_DIR}/cmake
)

# Imgui Part
set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui)
set(IMGUI_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/imgui)

include_directories(${IMGUI_SOURCE_DIR})
include_directories(${IMGUI_SOURCE_DIR}/backends)


add_library(imgui STATIC
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

install(TARGETS imgui
        DESTINATION ${IMGUI_INSTALL_DIR}/lib)

# Vulkan Part
find_package(Vulkan REQUIRED)

# Eigen Part
set(EIGEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/eigen)
include_directories(${EIGEN_SOURCE_DIR})

# stb Part
set(STB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb)
include_directories(${STB_SOURCE_DIR})

add_executable(main main.cpp)
target_link_libraries(main PUBLIC Vulkan::Vulkan)
target_link_libraries(main PRIVATE imgui)
target_link_libraries(main PRIVATE assimp)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)

