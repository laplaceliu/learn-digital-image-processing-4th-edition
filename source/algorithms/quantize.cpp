#include <algorithms/quantize.hpp>

namespace dip {
namespace algorithms {

Image quantize(const Image &img, int levels) {
  if (levels <= 0 || levels > 256) {
    throw std::invalid_argument("Levels must be between 1 and 256");
  }

  Image result(img.width(), img.height(), img.channels());

  // 计算量化步长
  int step = 256 / levels;

  for (int y = 0; y < img.height(); y++) {
    for (int x = 0; x < img.width(); x++) {
      for (int c = 0; c < img.channels(); c++) {
        uint8_t pixel = img.at<uint8_t>(y, x, c);
        // 量化：将像素值映射到最近的级别
        uint8_t quantized = (pixel / step) * step + step / 2;
        result.at<uint8_t>(y, x, c) = quantized;
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
