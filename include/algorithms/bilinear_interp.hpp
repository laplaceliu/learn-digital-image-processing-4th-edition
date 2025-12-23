#ifndef ALGORITHMS_BILINEAR_INTERP_HPP
#define ALGORITHMS_BILINEAR_INTERP_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 双线性插值函数
// @param img 输入图像
// @param x 浮点x坐标
// @param y 浮点y坐标
// @param channel 通道索引（默认为0）
// @return 插值后的像素值
uint8_t bilinear_interp(const Image &img, float x, float y, int channel = 0);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_BILINEAR_INTERP_HPP
