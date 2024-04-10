#using sudo cmake . and sudo make
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
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

# Vulkan Part
find_package(Vulkan REQUIRED)

# glfw Part
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw)
add_subdirectory(${GLFW_SOURCE_DIR})

# Imgui Part
set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui)
set(IMGUI_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/imgui)

include_directories(${IMGUI_SOURCE_DIR})
include_directories(${IMGUI_SOURCE_DIR}/backends)


add_library(imgui STATIC
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC ${GLFW_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC glfw)

install(TARGETS imgui
        DESTINATION ${IMGUI_INSTALL_DIR}/lib)


# Eigen Part
set(EIGEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/eigen)
include_directories(${EIGEN_SOURCE_DIR})

# stb Part
set(STB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb)
include_directories(${STB_SOURCE_DIR})

# glm Part
set(GLM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm)
#[[
set(GLM_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/glm)
add_subdirectory(${GLM_SOURCE_DIR})

install(TARGETS glm
        EXPORT GlmTargets
        ARCHIVE DESTINATION ${GLM_INSTALL_DIR}/lib
        LIBRARY DESTINATION ${GLM_INSTALL_DIR}/lib
        RUNTIME DESTINATION ${GLM_INSTALL_DIR}/bin
)


# 导出 Assimp 的 CMake 配置文件
install(EXPORT GlmTargets
        FILE GlmTargets.cmake
        DESTINATION ${GLM_INSTALL_DIR}/cmake
)
]]



add_executable(main main.cpp)

target_include_directories(main PRIVATE ${GLM_SOURCE_DIR})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCES src/*.cpp)
target_sources(main PRIVATE ${SOURCES})


target_link_libraries(main PUBLIC Vulkan::Vulkan)
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE imgui)
target_link_libraries(main PRIVATE assimp)

#target_link_libraries(main PRIVATE glm)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)

