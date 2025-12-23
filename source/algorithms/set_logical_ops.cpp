#include "algorithms/set_logical_ops.hpp"
#include "core/core.hpp"
#include <spdlog/spdlog.h>

namespace dip {

Image set_complement(const Image &img, int K) {
  spdlog::info("Applying set complement to {}x{} image with K={}", img.width(),
               img.height(), K);

  // 检查输入图像有效性
  if (img.empty()) {
    spdlog::error("Input image is empty");
    return Image();
  }

  // 创建结果图像
  Image result = img.clone();

  // 对每个像素进行补集运算
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      for (int c = 0; c < img.channels(); ++c) {
        uint8_t pixel_value = img.at<uint8_t>(y, x, c);
        result.at<uint8_t>(y, x, c) = static_cast<uint8_t>(K - pixel_value);
      }
    }
  }

  return result;
}

Image logical_and(const Image &img1, const Image &img2) {
  spdlog::info("Applying logical AND to {}x{} and {}x{} images", img1.width(),
               img1.height(), img2.width(), img2.height());

  // 检查输入图像有效性
  if (img1.empty() || img2.empty()) {
    spdlog::error("One or both input images are empty");
    return Image();
  }

  // 检查图像尺寸是否一致
  if (img1.width() != img2.width() || img1.height() != img2.height() ||
      img1.channels() != img2.channels()) {
    spdlog::error("Input images must have the same dimensions and channels");
    return Image();
  }

  // 创建结果图像
  Image result(img1.width(), img1.height(), img1.channels());

  // 对每个像素进行逻辑AND运算
  for (int y = 0; y < img1.height(); ++y) {
    for (int x = 0; x < img1.width(); ++x) {
      for (int c = 0; c < img1.channels(); ++c) {
        uint8_t pixel1 = img1.at<uint8_t>(y, x, c);
        uint8_t pixel2 = img2.at<uint8_t>(y, x, c);
        // 二值图逻辑AND：只有两个像素都为1时结果为1
        result.at<uint8_t>(y, x, c) = (pixel1 == 1 && pixel2 == 1) ? 1 : 0;
      }
    }
  }

  return result;
}

Image logical_xor(const Image &img1, const Image &img2) {
  spdlog::info("Applying logical XOR to {}x{} and {}x{} images", img1.width(),
               img1.height(), img2.width(), img2.height());

  // 检查输入图像有效性
  if (img1.empty() || img2.empty()) {
    spdlog::error("One or both input images are empty");
    return Image();
  }

  // 检查图像尺寸是否一致
  if (img1.width() != img2.width() || img1.height() != img2.height() ||
      img1.channels() != img2.channels()) {
    spdlog::error("Input images must have the same dimensions and channels");
    return Image();
  }

  // 创建结果图像
  Image result(img1.width(), img1.height(), img1.channels());

  // 对每个像素进行逻辑XOR运算
  for (int y = 0; y < img1.height(); ++y) {
    for (int x = 0; x < img1.width(); ++x) {
      for (int c = 0; c < img1.channels(); ++c) {
        uint8_t pixel1 = img1.at<uint8_t>(y, x, c);
        uint8_t pixel2 = img2.at<uint8_t>(y, x, c);
        // 二值图逻辑XOR：两个像素不相同时结果为1
        result.at<uint8_t>(y, x, c) = (pixel1 != pixel2) ? 1 : 0;
      }
    }
  }

  return result;
}

} // namespace dip
