#include <algorithms/bilinear_zoom.hpp>
#include <cmath>

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

        // 取4个邻域像素的整数坐标
        int y1 = static_cast<int>(std::floor(orig_y));
        int y2 = y1 + 1;
        int x1 = static_cast<int>(std::floor(orig_x));
        int x2 = x1 + 1;

        // 边界处理
        y2 = (y2 >= height) ? height - 1 : y2;
        x2 = (x2 >= width) ? width - 1 : x2;

        // 计算x轴、y轴权重（距离反比）
        float wx = orig_x - x1;
        float wy = orig_y - y1;

        // 获取4个邻域像素值
        uint8_t p11 = img.at<uint8_t>(y1, x1, c);
        uint8_t p12 = img.at<uint8_t>(y1, x2, c);
        uint8_t p21 = img.at<uint8_t>(y2, x1, c);
        uint8_t p22 = img.at<uint8_t>(y2, x2, c);

        // 双线性加权计算
        float val = (1.0f - wx) * (1.0f - wy) * p11 + (1.0f - wx) * wy * p12 +
                    wx * (1.0f - wy) * p21 + wx * wy * p22;

        result.at<uint8_t>(i, j, c) = static_cast<uint8_t>(val);
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
