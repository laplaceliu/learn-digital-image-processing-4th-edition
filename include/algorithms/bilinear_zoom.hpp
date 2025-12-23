#ifndef ALGORITHMS_BILINEAR_ZOOM_HPP
#define ALGORITHMS_BILINEAR_ZOOM_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 双线性插值放大
// @param img 输入图像
// @param scale 放大倍数
// @return 放大后的图像
Image bilinear_zoom(const Image &img, float scale);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_BILINEAR_ZOOM_HPP
