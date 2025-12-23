# 数字图像处理（第四版）实践项目

基于《数字图像处理（第四版）》教材的 C++ 实现项目，包含核心图像处理算法和实践示例。

## 🚀 项目简介

本项目使用现代 C++ (C++17) 实现了数字图像处理的核心算法，包括图像缩放、量化、降采样等基础操作。项目采用模块化设计，易于扩展和学习。

## 📁 项目结构

```text
learn-digital-image-processing-4th-edition/
├── include/                    # 头文件
│   ├── core/                  # 核心数据结构和工具
│   └── algorithms/            # 算法接口定义
├── source/                    # 源文件
│   ├── algorithms/            # 算法实现
│   ├── examples/              # 示例程序
│   └── common/                # 通用工具
├── external/                  # 外部依赖库
├── misc/                      # 资源文件
└── build/                     # 构建输出
```

## 🔧 开发指南

### 添加新算法

1. 在 `include/algorithms/` 中定义接口
2. 在 `source/algorithms/` 中实现算法
3. 在 `source/examples/` 中创建测试程序
4. CMake 会自动发现和构建新文件

详细开发指南请参考 [DEV.md](DEV.md)。
