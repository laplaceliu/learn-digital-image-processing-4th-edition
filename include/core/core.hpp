#ifndef CORE_CORE_HPP
#define CORE_CORE_HPP

// 核心数据结构和功能模块
// 这是一个header-only库，包含了所有必需的图像处理基础组件

#include "basic_types.hpp"
#include "vector_types.hpp"
#include "matrix.hpp"
#include "image.hpp"
#include "image_loader.hpp"

// 便利的命名空间别名
namespace core {
    // 基础类型
    using ::DataType;
    using ::Size;
    template<typename T> using Point = ::Point<T>;
    using ::Rect;
    
    // 向量类型
    template<typename T, int N> using Vec = ::Vec<T, N>;
    using ::Scalar;
    using ::Vec2b, ::Vec3b, ::Vec4b;
    using ::Vec2i, ::Vec3i, ::Vec4i;
    using ::Vec2f, ::Vec3f, ::Vec4f;
    using ::Vec2d, ::Vec3d, ::Vec4d;
    
    // 矩阵和图像
    using ::Matrix;
    using ::Image;
    
    // 矩阵运算
    namespace matrix_ops = ::matrix_ops;
    
    // 图像运算
    namespace image_ops = ::image_ops;
    
    // 图像加载
    using ::ImageLoader;
    namespace image_saver = ::image_saver;
}

// 常用类型的简化别名
namespace img = core;  // 图像处理命名空间

// 常用颜色常量
namespace colors {
    inline const Vec3b BLACK(0, 0, 0);
    inline const Vec3b WHITE(255, 255, 255);
    inline const Vec3b RED(255, 0, 0);
    inline const Vec3b GREEN(0, 255, 0);
    inline const Vec3b BLUE(0, 0, 255);
    inline const Vec3b YELLOW(255, 255, 0);
    inline const Vec3b CYAN(0, 255, 255);
    inline const Vec3b MAGENTA(255, 0, 255);
}

// 便利函数
namespace utils {
    
// 创建图像的便利函数
inline Image loadImage(const std::string& filename, int channels = 0) {
    auto img = ImageLoader::load_from_file(filename, channels);
    return img ? *img : Image();
}

inline bool saveImage(const Image& image, const std::string& filename) {
    return image_saver::save(image, filename);
}

inline bool saveImageBinary(const Image& image, const std::string& filename) {
    return image_saver::save_binary(image, filename);
}

// 创建常用图像
inline Image createColorImage(const Size& size, const Vec3b& color) {
    return image_ops::createColorImage(size, color);
}

inline Image createGrayImage(const Size& size, uint8_t value = 0) {
    return image_ops::createGrayImage(size, value);
}

// 图像操作便利函数
inline Image resize(const Image& src, const Size& new_size) {
    return image_ops::resize(src, new_size);
}

inline Image flipHorizontal(const Image& src) {
    return image_ops::flipHorizontal(src);
}

inline Image flipVertical(const Image& src) {
    return image_ops::flipVertical(src);
}

inline Image rotate90(const Image& src, bool clockwise = true) {
    return image_ops::rotate90(src, clockwise);
}

// 矩阵操作便利函数
inline Matrix zeros(int rows, int cols, DataType dtype = DataType::FLOAT32) {
    return matrix_ops::zeros(rows, cols, dtype);
}

inline Matrix zeros(const Size& size, DataType dtype = DataType::FLOAT32) {
    return matrix_ops::zeros(size, dtype);
}

inline Matrix eye(int size, DataType dtype = DataType::FLOAT32) {
    return matrix_ops::eye(size, dtype);
}

inline Matrix transpose(const Matrix& src) {
    return matrix_ops::transpose(src);
}

inline Matrix add(const Matrix& a, const Matrix& b) {
    return matrix_ops::add(a, b);
}

inline Matrix subtract(const Matrix& a, const Matrix& b) {
    return matrix_ops::subtract(a, b);
}

} // namespace utils

// 宏定义用于调试
#ifdef DEBUG
    #define PRINT_IMG_INFO(img) img.printInfo(#img)
    #define PRINT_MAT_INFO(mat) mat.printInfo(#mat)
#else
    #define PRINT_IMG_INFO(img)
    #define PRINT_MAT_INFO(mat)
#endif

// 安全的像素访问宏
#define SAFE_PIXEL(img, x, y, ch, default_val) \
    (((x) >= 0 && (x) < (img).width() && (y) >= 0 && (y) < (img).height() && (ch) >= 0 && (ch) < (img).channels()) ? \
     (img).at<uint8_t>((y), (x), (ch)) : (default_val))

// 遍历图像的便利宏
#define FOR_EACH_PIXEL(img) \
    for(int y = 0; y < (img).height(); ++y) \
        for(int x = 0; x < (img).width(); ++x)

#define FOR_EACH_PIXEL_CH(img) \
    FOR_EACH_PIXEL(img) \
        for(int c = 0; c < (img).channels(); ++c)

#endif // CORE_CORE_HPP