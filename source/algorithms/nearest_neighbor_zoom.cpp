#include <algorithms/nearest_neighbor_zoom.hpp>
#include <cmath>

namespace dip {
namespace algorithms {

Image nearest_neighbor_zoom(const Image &img, float scale) {
  if (scale <= 0.0f) {
    throw std::invalid_argument("Scale must be positive");
  }

  int new_height = static_cast<int>(img.height() * scale);
  int new_width = static_cast<int>(img.width() * scale);

  Image result(new_width, new_height, img.channels());

  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      // 映射到原始图像坐标
      float src_x = x / scale;
      float src_y = y / scale;

      // 最近邻取整
      int orig_x = static_cast<int>(std::round(src_x));
      int orig_y = static_cast<int>(std::round(src_y));

      // 边界检查
      orig_x = std::max(0, std::min(img.width() - 1, orig_x));
      orig_y = std::max(0, std::min(img.height() - 1, orig_y));

      // 复制所有通道
      for (int c = 0; c < img.channels(); c++) {
        result.at<uint8_t>(y, x, c) = img.at<uint8_t>(orig_y, orig_x, c);
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
