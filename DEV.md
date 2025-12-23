# 开发指南 (DEV.md)

本文档为《数字图像处理（第四版）》项目开发的详细指南，面向后续开发者。

## 项目组织结构

```text
learn-digital-image-processing-4th-edition/
├── CMakeLists.txt              # 主构建配置
├── README.md                   # 项目说明
├── DEV.md                      # 本文件 - 开发指南
├── external/                   # 外部依赖库
│   ├── spdlog/                 # 日志库
│   └── stb/                    # 图像加载库
├── include/                    # 头文件目录
│   ├── core/                   # 核心数据结构和工具
│   │   ├── basic_types.hpp     # 基础数据类型定义
│   │   ├── image.hpp           # 图像类定义
│   │   ├── image_loader.hpp    # 图像加载接口
│   │   ├── matrix.hpp          # 矩阵运算
│   │   ├── vector_types.hpp    # 向量类型定义
│   │   └── core.hpp            # 核心头文件入口
│   └── algorithms/             # 算法接口
├── source/                     # 源文件目录
│   ├── common/                 # 通用实现
│   │   └── stb_image_impl.cpp  # STB 图像库实现
│   ├── algorithms/             # 算法实现
│   ├── examples/               # 示例程序
│   └── tests/                  # 单元测试（待完善）
└── misc/                       # 资源文件
```

## 命名规则

### 1. 目录和文件命名

- **目录名**: 小写字母 + 下划线，如 `source/algorithms/`
- **头文件**: 小写字母 + 下划线 + `.hpp` 扩展名，如 `image_loader.hpp`
- **源文件**: 小写字母 + 下划线 + `.cpp` 扩展名，如 `bilinear_zoom.cpp`
- **示例文件**: 以 `test_` 或功能名开头，如 `test_zoom_algorithms.cpp`

### 2. 命名空间

- **主命名空间**: `dip` (Digital Image Processing)
- 所有公共 API 都应在 `dip` 命名空间内

### 3. 类和函数命名

- **类名**: PascalCase，如 `Image`, `ImageLoader`, `Matrix<T>`
- **函数名**: snake_case，如 `load_from_file()`, `nearest_neighbor_zoom()`
- **变量名**: snake_case，如 `scale_factor`, `output_image`
- **常量**: UPPER_SNAKE_CASE，如 `MAX_CHANNELS`, `DEFAULT_QUALITY`

### 4. 变量命名约定

为了提高代码可读性和一致性，项目采用以下变量命名约定：

- **类成员变量**: 末尾加下划线 (`member_var_`)

```cpp
class Image {
private:
  int width_;      // 类成员变量
  Matrix mat_;     // 类成员变量
  int channels_;   // 类成员变量
};
  ```

- **函数参数**: 末尾不加下划线 (`parameter`)

```cpp
void process_image(const Image& image, int scale_factor) {
  // image 和 scale_factor 是函数参数
}
```

- **局部变量**: 末尾不加下划线 (`local_var`)

```cpp
void calculate() {
  int result = 0;  // 局部变量
  for (int i = 0; i < count; ++i) {  // 局部变量
    result += data[i];
  }
}
```

- **全局变量/常量**: UPPER_SNAKE_CASE (`GLOBAL_CONSTANT`)

```cpp
static constexpr int MAX_PIXEL_VALUE = 255;
const std::string DEFAULT_FORMAT = "png";
```

**结构体 vs 类的命名差异**：

结构体通常有两种设计模式，采用不同的命名约定：

**简单数据聚合（POD 结构体）**：

```cpp
// 结构体主要是数据容器，成员变量不加下划线
struct Size {
  int width, height;  // 不加下划线

  Size(int w, int h) : width(w), height(h) {}
};

struct Point {
  T x, y;           // 不加下划线

  Point(T x, T y) : x(x), y(y) {}  // 参数名与成员不同，避免冲突
};
```

**复杂行为结构体**：

```cpp
// 结构体有复杂方法，成员变量加下划线
struct ComplexData {
  int data_count_;     // 加下划线
  double threshold_;

  bool process() { /* 复杂逻辑 */ }
};
```

**项目中的结构体约定**：

- **`Size`, `Point<T>`, `Rect`**: 简单数据聚合，成员不加下划线
- **构造函数参数**: 使用与成员不同的名称（如 `x_`, `y_`）避免名称冲突

这种命名方式的优点：

- **清晰区分**: 一眼就能看出变量的作用域
- **避免冲突**: 成员变量和参数名不会冲突
- **符合惯例**: 遵循 C++ 社区广泛采用的约定
- **便于维护**: 降低代码理解难度
- **语义明确**: 简单聚合 vs 复合类型有明显区别

### 5. 枚举和常量

```cpp
enum class DataType {
  UINT8, INT8, UINT16, INT16, FLOAT32, FLOAT64
};

static constexpr int MAX_CHANNELS = 4;
```

## 代码风格

### 1. 缩进和格式

- 使用 2 空格缩进，不使用 tab
- 大括号使用 Allman 风格（另起一行）
- 函数和类之间留一个空行

### 2. 注释规范

- 使用 `//` 进行单行注释，`/* */` 进行多行注释
- 公共接口应有详细的文档注释
- 复杂算法应在关键步骤添加解释性注释

## 如何扩展

### 1. 添加新算法

#### 步骤 1: 创建算法接口头文件

在 `include/algorithms/` 目录下创建头文件：

```cpp
// include/algorithms/your_algorithm.hpp
#ifndef ALGORITHMS_YOUR_ALGORITHM_HPP
#define ALGORITHMS_YOUR_ALGORITHM_HPP

#include "core/image.hpp"

namespace dip {

Image your_algorithm(const Image& input, /* 参数 */);

} // namespace dip

#endif // ALGORITHMS_YOUR_ALGORITHM_HPP
```

#### 步骤 2: 实现算法

在 `source/algorithms/` 目录下创建实现文件：

```cpp
// source/algorithms/your_algorithm.cpp
#include "algorithms/your_algorithm.hpp"
#include "core/core.hpp"
#include <spdlog/spdlog.h>

namespace dip {

Image your_algorithm(const Image& input, /* 参数 */) {
  spdlog::info("Applying your algorithm to {}x{} image",
               input.width(), input.height());

  // 实现算法逻辑
  Image result = input.clone();

  // 处理逻辑...

  return result;
}

} // namespace dip
```

#### 步骤 3: 创建测试程序

在 `source/examples/` 目录下创建测试文件：

```cpp
// source/examples/test_your_algorithm.cpp
#include "algorithms/your_algorithm.hpp"
#include "core/core.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  std::string filename = argc > 1 ? argv[1] : "test.jpg";
  auto img = ImageLoader::load_from_file(filename);

  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  // 测试算法
  auto result = your_algorithm(*img, /* 参数 */);

  // 保存或显示结果
  // ImageLoader::save_to_file("result.jpg", result);

  spdlog::info("Algorithm test completed!");
  return 0;
}
```

#### 步骤 4: 更新 CMakeLists.txt

CMakeLists.txt 会自动扫描 `source/algorithms/` 和 `source/examples/` 目录，无需手动配置。

### 2. 添加新核心数据类型

1. 在 `include/core/basic_types.hpp` 中添加新的枚举或类型定义
2. 在相应的头文件中声明新的类
3. 在 `source/common/` 或适当目录中实现

### 3. 添加新的图像格式支持

在 `source/common/stb_image_impl.cpp` 中扩展 STB 库的配置，或创建新的加载器。

## 开发工作流

### 1. 构建项目

```bash
# 配置（首次）
cmake -S . -B build

# 构建
cmake --build build

# 运行示例
cd build
./test_zoom_algorithms test.jpg 2.5
```

### 2. 调试和测试

- 所有构建产物在 `build/` 目录
- 测试图片 `test.jpg` 会自动复制到构建目录
- 使用 `spdlog` 进行日志输出
- 支持通过命令行参数指定输入文件

### 3. 代码风格

- 使用 2 空格缩进
- 头文件包含保护使用 `#ifndef` 模式
- 所有函数和类应有适当的文档注释
- 遵循 RAII 原则管理资源

## 项目配置

### VS Code 设置

- 文件排除规则在 `.vscode/settings.json` 的 `files.exclude` 中配置
- IntelliSense 路径自动包含 `include/`, `external/` 等目录
- C++ 标准设置为 C++17

### CMake 配置

- 最低版本要求：CMake 3.10
- C++ 标准：C++17
- 自动生成 `compile_commands.json` 支持 IDE

### 依赖管理

- **spdlog**: 日志库，作为 Git 子模块在 `external/spdlog`
- **stb**: 图像加载库，在 `external/stb`
- 新增外部库建议放在 `external/` 目录

## 重要注意事项

### 1. 资源文件管理

- 测试图片等资源放在 `misc/` 目录
- CMake 会自动复制 `misc/test.jpg` 到构建目录
- 其他资源文件需要手动在 CMakeLists.txt 中添加复制规则

### 2. 内存管理

- 使用 RAII 原则
- Image 类自动管理内存
- 避免裸指针，优先使用智能指针

### 3. 错误处理

- 使用异常处理严重错误
- 使用 spdlog 记录错误信息
- 提供有意义的错误消息

### 4. 性能考虑

- 大图像处理时注意内存使用
- 可考虑并行化处理
- 使用适当的数据类型（如 UINT8 vs FLOAT32）

## 为 AI 助手的特别提示

1. **CMakeLists.txt 是自动的**：新增算法文件时无需修改构建配置
2. **头文件包含顺序**：先包含标准库，然后是第三方库，最后是项目头文件
3. **测试驱动开发**：每次添加新功能时，创建对应的测试程序
4. **向后兼容**：修改核心 API 时考虑向后兼容性
5. **文档同步更新**：修改接口时同步更新相关文档

## 常见问题解决

### Q: VS Code 不显示某些文件或目录？

A: 检查 `.vscode/settings.json` 中的 `files.exclude` 配置。

### Q: 找不到头文件？

A: 确保 CMake 已重新配置，检查 `build/compile_commands.json` 是否生成。

### Q: 程序运行时找不到 test.jpg？

A: 确保 CMake 已重新构建，测试图片会自动复制到 `build/` 目录。

### Q: 如何添加新的外部依赖？

A: 将库放在 `external/` 目录，然后在 CMakeLists.txt 中添加相应的 `add_subdirectory()` 和链接配置。
