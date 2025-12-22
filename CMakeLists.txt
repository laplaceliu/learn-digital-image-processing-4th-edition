cmake_minimum_required(VERSION 3.10)
project(learn-digital-image-processing-4th-edition)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置包含目录
include_directories(include external)

# 为了更好地支持尖括号包含，设置全局包含路径
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)

# 收集公共源文件
file(GLOB COMMON_SOURCES "source/*.cpp")

# 为每个章节目录创建可执行文件
file(GLOB CHAPTER_DIRS "source/chapter*")
foreach(CHAPTER_DIR ${CHAPTER_DIRS})
    file(GLOB CHAPTER_SOURCES "${CHAPTER_DIR}/*.cpp")
    foreach(SOURCE_FILE ${CHAPTER_SOURCES})
        # 获取文件名（不含路径和扩展名）
        get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)
        # 创建可执行文件
        add_executable(${EXEC_NAME} ${SOURCE_FILE} ${COMMON_SOURCES})
    endforeach()
endforeach()