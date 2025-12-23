cmake_minimum_required(VERSION 3.10)
project(learn-digital-image-processing-4th-edition)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成compile_commands.json以支持IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置包含目录
include_directories(include external external/spdlog/include)

# 为了更好地支持尖括号包含，设置全局包含路径
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)

# 添加spdlog子目录
add_subdirectory(external/spdlog)

# 收集公共源文件和算法源文件（算法库）
file(GLOB COMMON_SOURCES "source/common/*.cpp" "source/algorithms/*.cpp")

# 创建算法库
add_library(image_algorithms STATIC ${COMMON_SOURCES})
target_link_libraries(image_algorithms spdlog::spdlog)

# 示例和测试可执行文件
file(GLOB_RECURSE EXAMPLE_SOURCES "source/examples/*.cpp")
foreach(SOURCE_FILE ${EXAMPLE_SOURCES})
    # 获取文件名（不含路径和扩展名）
    get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)
    # 创建可执行文件
    add_executable(${EXEC_NAME} ${SOURCE_FILE})
    # 链接算法库和spdlog
    target_link_libraries(${EXEC_NAME} image_algorithms spdlog::spdlog)
endforeach()

# 复制测试图片到构建目录
add_custom_command(TARGET image_algorithms POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/misc/test.png
    ${CMAKE_BINARY_DIR}/test.png
    COMMENT "Copying test.png to build directory")
