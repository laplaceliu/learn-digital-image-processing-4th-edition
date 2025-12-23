#include <algorithms/bilinear_interp.hpp>
#include <algorithms/rotate.hpp>
#include <cmath>

namespace dip {
namespace algorithms {

Image rotate(const Image &img, double theta) {
  int height = img.height();
  int width = img.width();
  int channels = img.channels();

  // 创建输出图像（保持原始尺寸）
  Image result(width, height, channels);

  double cos_t = std::cos(theta);
  double sin_t = std::sin(theta);

  // 图像中心坐标
  double cx = width / 2.0;
  double cy = height / 2.0;

  // 遍历输出图像的每个像素
  for (int y_out = 0; y_out < height; y_out++) {
    for (int x_out = 0; x_out < width; x_out++) {
      // 逆映射：从输出坐标反推原坐标
      double x = (x_out - cx) * cos_t + (y_out - cy) * sin_t + cx;
      double y = -(x_out - cx) * sin_t + (y_out - cy) * cos_t + cy;

      // 检查原坐标是否在图像范围内
      if (x >= 0 && x < width && y >= 0 && y < height) {
        // 对每个通道进行双线性插值
        for (int c = 0; c < channels; c++) {
          result.at<uint8_t>(y_out, x_out, c) = bilinear_interp(img, x, y, c);
        }
      } else {
        // 超出范围设为黑色（0）
        for (int c = 0; c < channels; c++) {
          result.at<uint8_t>(y_out, x_out, c) = 0;
        }
      }
    }
  }

  return result;
}

} // namespace algorithms
} // namespace dip
