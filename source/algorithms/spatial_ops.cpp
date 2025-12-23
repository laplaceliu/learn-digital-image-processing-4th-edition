#include <algorithms/spatial_ops.hpp>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

namespace dip {

Image invert_image(const Image &img, int max_gray) {
  spdlog::info("Applying image inversion to {}x{} image with max_gray={}",
               img.width(), img.height(), max_gray);

  // 检查输入图像有效性
  if (img.empty()) {
    spdlog::error("Input image is empty");
    return Image();
  }

  // 创建结果图像
  Image result = img.clone();

  // 对每个像素进行求反运算：res[i][j] = maxGray - img[i][j]
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      for (int c = 0; c < img.channels(); ++c) {
        uint8_t pixel_value = img.at<uint8_t>(y, x, c);
        result.at<uint8_t>(y, x, c) =
            static_cast<uint8_t>(max_gray - pixel_value);
      }
    }
  }

  return result;
}

} // namespace dip
