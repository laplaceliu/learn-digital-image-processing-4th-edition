#ifndef CORE_CORE_HPP
#define CORE_CORE_HPP

// 核心数据结构和功能模块
// 这是一个header-only库，包含了所有必需的图像处理基础组件

#include "basic_types.hpp"
#include "image.hpp"
#include "image_loader.hpp"
#include "matrix.hpp"
#include "vector_types.hpp"

// 导出所有核心类型到dip命名空间
namespace dip {

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
} // namespace colors

// 宏定义用于调试
#ifdef DEBUG
#define PRINT_IMG_INFO(img) img.printInfo(#img)
#define PRINT_MAT_INFO(mat) mat.printInfo(#mat)
#else
#define PRINT_IMG_INFO(img)
#define PRINT_MAT_INFO(mat)
#endif

// 安全的像素访问宏
#define SAFE_PIXEL(img, x, y, ch, default_val)                                 \
  (((x) >= 0 && (x) < (img).width() && (y) >= 0 && (y) < (img).height() &&     \
    (ch) >= 0 && (ch) < (img).channels())                                      \
       ? (img).at<uint8_t>((y), (x), (ch))                                     \
       : (default_val))

// 遍历图像的便利宏
#define FOR_EACH_PIXEL(img)                                                    \
  for (int y = 0; y < (img).height(); ++y)                                     \
    for (int x = 0; x < (img).width(); ++x)

#define FOR_EACH_PIXEL_CH(img)                                                 \
  FOR_EACH_PIXEL(img)                                                          \
  for (int c = 0; c < (img).channels(); ++c)

} // namespace dip

#endif // CORE_CORE_HPP
