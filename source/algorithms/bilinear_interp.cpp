#include <algorithm>
#include <algorithms/bilinear_interp.hpp>
#include <stdexcept>

namespace dip {
namespace algorithms {

uint8_t bilinear_interp(const Image &img, float x, float y, int channel) {
  int height = img.height();
  int width = img.width();

  // 检查通道有效性
  if (channel < 0 || channel >= img.channels()) {
    throw std::out_of_range("Channel index out of range");
  }

  // 取邻域4个整数坐标
  int x1 = static_cast<int>(std::floor(x));
  int y1 = static_cast<int>(std::floor(y));
  int x2 = x1 + 1;
  int y2 = y1 + 1;

  // 边界处理
  x1 = std::max(0, std::min(x1, width - 1));
  x2 = std::max(0, std::min(x2, width - 1));
  y1 = std::max(0, std::min(y1, height - 1));
  y2 = std::max(0, std::min(y2, height - 1));

  // 获取4个邻域像素值
  uint8_t p11 = img.at<uint8_t>(y1, x1, channel);
  uint8_t p12 = img.at<uint8_t>(y1, x2, channel);
  uint8_t p21 = img.at<uint8_t>(y2, x1, channel);
  uint8_t p22 = img.at<uint8_t>(y2, x2, channel);

  // 计算x轴、y轴权重（距离反比）
  float wx = x - x1;
  float wy = y - y1;

  // 双线性加权计算
  float val = (1.0f - wx) * (1.0f - wy) * p11 + (1.0f - wx) * wy * p12 +
              wx * (1.0f - wy) * p21 + wx * wy * p22;

  return static_cast<uint8_t>(val);
}

} // namespace algorithms
} // namespace dip
