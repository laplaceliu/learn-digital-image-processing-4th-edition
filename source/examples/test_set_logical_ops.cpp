#include "algorithms/set_logical_ops.hpp"
#include "core/core.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  // 创建测试图像
  spdlog::info("Creating test images...");

  // 创建一个简单的测试二值图1 (3x3)
  dip::Image img1(3, 3, 1);
  img1.at<uint8_t>(0, 0, 0) = 0;
  img1.at<uint8_t>(0, 1, 0) = 1;
  img1.at<uint8_t>(0, 2, 0) = 0;
  img1.at<uint8_t>(1, 0, 0) = 1;
  img1.at<uint8_t>(1, 1, 0) = 1;
  img1.at<uint8_t>(1, 2, 0) = 1;
  img1.at<uint8_t>(2, 0, 0) = 0;
  img1.at<uint8_t>(2, 1, 0) = 1;
  img1.at<uint8_t>(2, 2, 0) = 0;

  // 创建一个简单的测试二值图2 (3x3)
  dip::Image img2(3, 3, 1);
  img2.at<uint8_t>(0, 0, 0) = 1;
  img2.at<uint8_t>(0, 1, 0) = 1;
  img2.at<uint8_t>(0, 2, 0) = 0;
  img2.at<uint8_t>(1, 0, 0) = 0;
  img2.at<uint8_t>(1, 1, 0) = 1;
  img2.at<uint8_t>(1, 2, 0) = 0;
  img2.at<uint8_t>(2, 0, 0) = 0;
  img2.at<uint8_t>(2, 1, 0) = 1;
  img2.at<uint8_t>(2, 2, 0) = 1;

  // 创建一个简单的测试灰度图 (3x3)
  dip::Image gray_img(3, 3, 1);
  gray_img.at<uint8_t>(0, 0, 0) = 50;
  gray_img.at<uint8_t>(0, 1, 0) = 100;
  gray_img.at<uint8_t>(0, 2, 0) = 200;
  gray_img.at<uint8_t>(1, 0, 0) = 150;
  gray_img.at<uint8_t>(1, 1, 0) = 75;
  gray_img.at<uint8_t>(1, 2, 0) = 25;
  gray_img.at<uint8_t>(2, 0, 0) = 180;
  gray_img.at<uint8_t>(2, 1, 0) = 90;
  gray_img.at<uint8_t>(2, 2, 0) = 120;

  std::cout << "=== 测试图像1 (二值图) ===" << std::endl;
  for (int y = 0; y < img1.height(); ++y) {
    for (int x = 0; x < img1.width(); ++x) {
      std::cout << static_cast<int>(img1.at<uint8_t>(y, x, 0)) << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "\n=== 测试图像2 (二值图) ===" << std::endl;
  for (int y = 0; y < img2.height(); ++y) {
    for (int x = 0; x < img2.width(); ++x) {
      std::cout << static_cast<int>(img2.at<uint8_t>(y, x, 0)) << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "\n=== 灰度图 ===" << std::endl;
  for (int y = 0; y < gray_img.height(); ++y) {
    for (int x = 0; x < gray_img.width(); ++x) {
      std::cout << static_cast<int>(gray_img.at<uint8_t>(y, x, 0)) << " ";
    }
    std::cout << std::endl;
  }

  // 测试集合补集运算
  std::cout << "\n=== 灰度图补集 (K=255) ===" << std::endl;
  auto complement_result = dip::set_complement(gray_img, 255);
  for (int y = 0; y < complement_result.height(); ++y) {
    for (int x = 0; x < complement_result.width(); ++x) {
      std::cout << static_cast<int>(complement_result.at<uint8_t>(y, x, 0))
                << " ";
    }
    std::cout << std::endl;
  }

  // 测试逻辑AND运算
  std::cout << "\n=== 逻辑AND结果 ===" << std::endl;
  auto and_result = dip::logical_and(img1, img2);
  for (int y = 0; y < and_result.height(); ++y) {
    for (int x = 0; x < and_result.width(); ++x) {
      std::cout << static_cast<int>(and_result.at<uint8_t>(y, x, 0)) << " ";
    }
    std::cout << std::endl;
  }

  // 测试逻辑XOR运算
  std::cout << "\n=== 逻辑XOR结果 ===" << std::endl;
  auto xor_result = dip::logical_xor(img1, img2);
  for (int y = 0; y < xor_result.height(); ++y) {
    for (int x = 0; x < xor_result.width(); ++x) {
      std::cout << static_cast<int>(xor_result.at<uint8_t>(y, x, 0)) << " ";
    }
    std::cout << std::endl;
  }

  spdlog::info("Set and logical operations test completed!");
  return 0;
}
