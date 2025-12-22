# 数字图像处理（第4版）学习项目

这是一个基于《数字图像处理（第4版）》教材的C++学习项目，使用CMake构建系统。

## 项目结构

```
.
├── CMakeLists.txt        # CMake配置文件
├── include/              # 头文件目录
│   └── core/           # 核心模块化库（header-only）
│       ├── basic_types.hpp    # 基础数据类型
│       ├── vector_types.hpp    # 向量类型
│       ├── matrix.hpp         # 矩阵类
│       ├── image.hpp          # 图像类
│       ├── image_loader.hpp   # 图像加载
│       └── core.hpp          # 总头文件
├── source/              # 源代码目录
│   ├── image_loader.cpp # 图像加载实现（STB_IMAGE_IMPLEMENTATION）
│   └── chapter02/      # 第2章算法实现
│       ├── nearest_neighbor_zoom.cpp  # 最近邻插值
│       ├── data_structure_demo.cpp    # 数据结构演示
│       └── modular_demo.cpp           # 模块化设计演示
├── external/            # 外部库
│   └── stb/            # STB图像库（单头文件）
├── build/              # 构建输出目录
└── README.md           # 项目说明
```

## 特性

- **Header-Only设计**：核心库无需单独编译，即插即用
- **模块化结构**：功能分离，便于维护和扩展
- **现代C++17**：使用模板、RAII、智能指针等现代特性
- **类型安全**：基于模板的类型系统，避免运行时类型错误
- **高性能**：连续内存布局，零拷贝设计
- **便利性**：丰富的工具函数和宏定义

## 核心模块详解

### 1. basic_types.hpp
```cpp
// 基础数据类型
enum class DataType { UINT8, FLOAT32, ... };
struct Size { int width, height; };
template<typename T> struct Point { T x, y; };
struct Rect { int x, y, width, height; };
```

### 2. vector_types.hpp
```cpp
// 固定大小向量（像素值）
template<typename T, int N> class Vec { ... };
class Scalar { double val[4]; };

// 常用类型别名
using Vec3b = Vec<uint8_t, 3>;
using Vec3f = Vec<float, 3>;
```

### 3. matrix.hpp
```cpp
// 通用矩阵类
class Matrix {
    int rows_, cols_;
    DataType dtype_;
    std::vector<uint8_t> data_;
    // ...
};
```

### 4. image.hpp
```cpp
// 专门的图像类
class Image {
    Matrix mat_;
    int channels_;
    // ROI、通道操作、几何变换等
};
```

### 5. image_loader.hpp
```cpp
// 图像I/O操作
class ImageLoader {
    static std::shared_ptr<Image> load_from_file(...);
    static bool save_as_ppm(...);
};
```

## 使用方式

### 基础用法
```cpp
#include "core/core.hpp"  // 包含所有功能

using namespace core;
using namespace utils;

// 加载图像
Image img = loadImage("test.jpg");
if (!img.empty()) {
    std::cout << "Loaded: " << img.width() << "x" << img.height() << std::endl;
}

// 创建图像
Image gray = createGrayImage(Size(640, 480), 128);
Image color = createColorImage(Size(100, 100), colors::RED);

// 像素操作
img.set_pixel(x, y, 0, 255);
uint8_t pixel = img.get_pixel(x, y, 0);

// 图像变换
Image flipped = flipHorizontal(img);
Image resized = resize(img, Size(320, 240));
```

### 高级用法
```cpp
// 矩阵操作
Matrix mat = matrix_ops::zeros(3, 3, DataType::FLOAT32);
Matrix identity = matrix_ops::eye(3);
Matrix sum = matrix_ops::add(a, b);

// 向量操作
Vec3f vec(1.0f, 2.0f, 3.0f);
Vec3f normalized = vec.normalized();
float length = vec.length();
Vec3f doubled = vec * 2.0f;

// 通道操作
Image red_channel = img.channel(0);
std::vector<Image> channels = img.splitChannels();
Image merged = Image::mergeChannels(channels);

// ROI操作
Rect roi(10, 10, 100, 100);
Image sub = img.pixel_roi(roi);
```

### 便利宏
```cpp
// 遍历像素
FOR_EACH_PIXEL(img) {
    // 处理像素 (x, y)
}

FOR_EACH_PIXEL_CH(img) {
    // 处理像素 (x, y) 和所有通道 c
}

// 安全访问
uint8_t pixel = SAFE_PIXEL(img, x, y, 0, 0);

// 调试
PRINT_IMG_INFO(img);
PRINT_MAT_INFO(mat);
```

## 编译和运行

### 编译

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 运行特定章节的程序

```bash
# 运行第2章的最近邻插值程序
./nearest_neighbor_zoom
```

## 章节内容

### 第2章 - 空间域图像增强

#### 2.4节 - 最近邻插值
实现图像的最近邻插值放大算法，支持任意倍数放大。

**程序：** `nearest_neighbor_zoom`
**功能：** 将图像放大10倍
**输入：** test.jpg
**输出：** zoomed_test.pgm

```cpp
Image nearestNeighborZoom(const Image& img, float scale);
```

## 依赖

- CMake 3.10+
- C++17兼容的编译器（GCC 7+, Clang 5+, MSVC 2017+）

## 图像格式支持

支持加载以下图像格式：
- JPEG (.jpg, .jpeg)
- PNG (.png)
- BMP (.bmp)
- TGA (.tga)
- PSD (.psd)
- GIF (.gif)
- HDR (.hdr)
- PIC (.pic)
- PNM (.ppm, .pgm)

## 使用示例

```cpp
#include "image_loader.h"

// 加载图像
auto img = ImageLoader::load_from_file("test.jpg");
if (img && img->is_valid()) {
    std::cout << "Loaded: " << img->width << "x" << img->height 
              << ", " << img->channels << " channels" << std::endl;
    
    // 访问像素数据
    unsigned char pixel = img->get_pixel(x, y, channel);
    img->set_pixel(x, y, channel, value);
    
    // 处理图像...
}
```

## 添加新章节

要添加新的章节代码：

1. 在`source/`目录下创建`chapterXX/`目录
2. 创建新的`.cpp`文件实现算法
3. 重新编译，CMake会自动识别新程序

```bash
mkdir source/chapter03
# 在chapter03中创建新的算法文件
cd build && cmake .. && cmake --build .
```

## 项目说明

本项目为学习数字图像处理算法提供完整的实现框架，包含：
- 现代化的图像数据结构
- 灵活的构建系统
- 按章节组织的代码结构
- 实用的图像I/O功能

## 贡献

欢迎提交Issue和Pull Request来改进这个学习项目。