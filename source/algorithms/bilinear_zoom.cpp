#include <algorithms/bilinear_interp.hpp>
#include <algorithms/bilinear_zoom.hpp>

namespace dip {
namespace algorithms {

Image bilinear_zoom(const Image &img, float scale) {
  if (scale <= 0.0f) {
    throw std::invalid_argument("Scale must be positive");
  }

  int height = img.height();
  int width = img.width();
  int channels = img.channels();

  int new_height = static_cast<int>(height * scale);
  int new_width = static_cast<int>(width * scale);

  Image result(new_width, new_height, channels);

  for (int c = 0; c < channels; c++) {
    for (int i = 0; i < new_height; i++) {
      for (int j = 0; j < new_width; j++) {
        // 映射到原始图像浮点坐标
        float orig_y = i / scale;
        float orig_x = j / scale;

        // 使用公共的双线性插值函数
        result.at<uint8_t>(i, j, c) = bilinear_interp(img, orig_x, orig_y, c);
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
