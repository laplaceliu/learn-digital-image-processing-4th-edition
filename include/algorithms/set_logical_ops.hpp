#ifndef ALGORITHMS_SET_LOGICAL_OPS_HPP
#define ALGORITHMS_SET_LOGICAL_OPS_HPP

#include "core/image.hpp"

namespace dip {

// 集合运算：灰度图补集（K=255）
Image set_complement(const Image &img, int K = 255);

// 逻辑运算：二值图AND
Image logical_and(const Image &img1, const Image &img2);

// 逻辑运算：二值图XOR
Image logical_xor(const Image &img1, const Image &img2);

} // namespace dip

#endif // ALGORITHMS_SET_LOGICAL_OPS_HPP
