#using sudo cmake . and sudo make
cmake_minimum_required(VERSION 3.16)

project(main)
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

find_package(Vulkan REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main assimp Vulkan::Vulkan)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)

