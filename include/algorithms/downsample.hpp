#ifndef ALGORITHMS_DOWNSAMPLE_HPP
#define ALGORITHMS_DOWNSAMPLE_HPP

#include <core/core.hpp>

namespace dip {
namespace algorithms {

// 图像下采样：使用邻域平均法缩小图像
// @param img 输入图像
// @param factor 下采样因子（>=2）
// @return 下采样后的图像
Image downsample(const Image &img, int factor);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_DOWNSAMPLE_HPP
