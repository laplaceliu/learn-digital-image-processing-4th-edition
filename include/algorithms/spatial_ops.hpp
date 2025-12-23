#ifndef ALGORITHMS_SPATIAL_OPS_HPP
#define ALGORITHMS_SPATIAL_OPS_HPP

#include "core/image.hpp"

namespace dip {

// 图像求反算法（空间域操作）
Image invert_image(const Image &img, int max_gray = 255);

} // namespace dip

#endif // ALGORITHMS_SPATIAL_OPS_HPP
