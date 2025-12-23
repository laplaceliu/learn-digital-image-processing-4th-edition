#include <algorithms/downsample.hpp>

namespace dip {
namespace algorithms {

Image downsample(const Image &img, int factor) {
  if (factor <= 1) {
    return img; // 不需要下采样
  }

  int new_width = img.width() / factor;
  int new_height = img.height() / factor;

  Image result(new_width, new_height, img.channels());

  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      for (int c = 0; c < img.channels(); c++) {
        int sum = 0;
        int count = 0;

        // 计算邻域平均值
        for (int dy = 0; dy < factor; dy++) {
          for (int dx = 0; dx < factor; dx++) {
            int src_x = x * factor + dx;
            int src_y = y * factor + dy;

            if (src_x < img.width() && src_y < img.height()) {
              sum += img.at<uint8_t>(src_y, src_x, c);
              count++;
            }
          }
        }

        result.at<uint8_t>(y, x, c) = static_cast<uint8_t>(sum / count);
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
