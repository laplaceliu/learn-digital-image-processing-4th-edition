#ifndef ALGORITHMS_NEAREST_NEIGHBOR_ZOOM_HPP
#define ALGORITHMS_NEAREST_NEIGHBOR_ZOOM_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 最近邻插值放大
// @param img 输入图像
// @param scale 放大倍数
// @return 放大后的图像
Image nearest_neighbor_zoom(const Image &img, float scale);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_NEAREST_NEIGHBOR_ZOOM_HPP
