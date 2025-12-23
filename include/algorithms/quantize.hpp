#ifndef ALGORITHMS_QUANTIZE_HPP
#define ALGORITHMS_QUANTIZE_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 图像量化：将像素值从256级减少到指定级别
// @param img 输入图像
// @param levels 量化级别数（1-256）
// @return 量化后的图像
Image quantize(const Image &img, int levels);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_QUANTIZE_HPP
