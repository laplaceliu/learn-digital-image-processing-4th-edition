#ifndef ALGORITHMS_ROTATE_HPP
#define ALGORITHMS_ROTATE_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 图像旋转（逆时针旋转，基于逆映射）
// @param img 输入图像
// @param theta 旋转角度（弧度）
// @return 旋转后的图像
Image rotate(const Image &img, double theta);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_ROTATE_HPP
