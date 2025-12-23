#include <algorithms/spatial_ops.hpp>
#include <core/core.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  std::string filename = "test.png";
  if (argc > 1) {
    filename = argv[1];
  }

  spdlog::info("Loading image: {}", filename);

  // 加载测试图像
  auto img_result = dip::ImageLoader::load_from_file(filename);
  if (!img_result || img_result->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  dip::Image img = *img_result;
  spdlog::info("Image loaded successfully: {}x{}x{}", img.width(), img.height(),
               img.channels());

  // 显示原始图像的一些像素信息
  spdlog::info("Original image pixel samples:");
  for (int y = 0; y < std::min(3, img.height()); ++y) {
    for (int x = 0; x < std::min(3, img.width()); ++x) {
      std::string pixel_str = "(";
      for (int c = 0; c < img.channels(); ++c) {
        pixel_str += std::to_string(static_cast<int>(img.at<uint8_t>(y, x, c)));
        if (c < img.channels() - 1)
          pixel_str += ",";
      }
      pixel_str += ") ";
      std::cout << "Pixel(" << y << "," << x << "): " << pixel_str << "\t";
    }
    std::cout << std::endl;
  }

  // 测试图像求反算法
  spdlog::info("Applying image inversion...");
  auto inverted_img = dip::invert_image(img, 255);

  if (inverted_img.empty()) {
    spdlog::error("Image inversion failed");
    return 1;
  }

  // 显示求反后图像的像素信息
  spdlog::info("Inverted image pixel samples:");
  for (int y = 0; y < std::min(3, inverted_img.height()); ++y) {
    for (int x = 0; x < std::min(3, inverted_img.width()); ++x) {
      std::string pixel_str = "(";
      for (int c = 0; c < inverted_img.channels(); ++c) {
        pixel_str +=
            std::to_string(static_cast<int>(inverted_img.at<uint8_t>(y, x, c)));
        if (c < inverted_img.channels() - 1)
          pixel_str += ",";
      }
      pixel_str += ") ";
      std::cout << "Pixel(" << y << "," << x << "): " << pixel_str << "\t";
    }
    std::cout << std::endl;
  }

  // 验证求反结果的正确性
  spdlog::info("Verifying inversion correctness...");
  bool verification_passed = true;
  int sample_points = 0;

  // 检查几个采样点
  for (int y = 0; y < std::min(5, img.height()); y += 2) {
    for (int x = 0; x < std::min(5, img.width()); x += 2) {
      for (int c = 0; c < img.channels(); ++c) {
        uint8_t original = img.at<uint8_t>(y, x, c);
        uint8_t inverted = inverted_img.at<uint8_t>(y, x, c);
        uint8_t expected = static_cast<uint8_t>(255 - original);

        if (inverted != expected) {
          spdlog::error("Verification failed at ({},{},{}): original={}, "
                        "inverted={}, expected={}",
                        x, y, c, static_cast<int>(original),
                        static_cast<int>(inverted), static_cast<int>(expected));
          verification_passed = false;
        } else {
          sample_points++;
        }
      }
    }
  }

  if (verification_passed) {
    spdlog::info(
        "Image inversion verification passed! Checked {} sample points.",
        sample_points);
  } else {
    spdlog::error("Image inversion verification failed!");
  }

  // 尝试保存求反后的图像（保存为PPM格式）
  std::string output_filename = "inverted_test.ppm";
  if (dip::image_saver::save_binary(inverted_img, output_filename)) {
    spdlog::info("Inverted image saved to: {}", output_filename);
  } else {
    spdlog::error("Failed to save inverted image");
  }

  spdlog::info("Image inversion test completed!");
  return verification_passed ? 0 : 1;
}
