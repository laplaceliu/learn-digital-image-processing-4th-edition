#include <core/core.hpp>
#include <spdlog/spdlog.h>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace dip;

namespace dip {
namespace test_memory_layout {

// 打印内存布局
void print_memory_layout(const Image &img, const std::string &name) {
  spdlog::info("=== {} ===", name);
  spdlog::info("尺寸: {}×{}×{}", img.width(), img.height(), img.channels());
  spdlog::info("数据类型: {}", dataTypeName(img.type()));
  spdlog::info("Matrix列数: {}", img.matrix().cols());
  spdlog::info("Matrix行数: {}", img.matrix().rows());
  spdlog::info("每行字节数: {}", img.matrix().step());
  spdlog::info("总数据大小: {} 元素", img.matrix().total());

  // 打印前几个字节的内存内容
  const uint8_t* data = img.ptr<uint8_t>(0);
  std::stringstream hex_stream;
  hex_stream << std::hex << std::setfill('0');
  for (int i = 0; i < std::min(16, static_cast<int>(img.matrix().total()));
       ++i) {
    hex_stream << std::setw(2) << static_cast<int>(data[i]) << " ";
  }
  spdlog::info("前16字节内存内容: {}", hex_stream.str());
}

// 演示像素访问的不同方法
void demonstrate_pixel_access() {
  spdlog::info("=== 像素访问方法演示 ===");

  // 创建一个小的RGB图像用于演示
  Image img(4, 3, 3);  // 4×3 RGB图像

  // 填充测试数据
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      img.at<uint8_t>(y, x, 0) = (x + y * 4) * 3;      // R
      img.at<uint8_t>(y, x, 1) = (x + y * 4) * 3 + 1;  // G
      img.at<uint8_t>(y, x, 2) = (x + y * 4) * 3 + 2;  // B
    }
  }

  print_memory_layout(img, "4×3 RGB测试图像");

  // 方法1: 直接访问
  spdlog::info("方法1 - 直接访问像素(1,2):");
  spdlog::info("R: {}", static_cast<int>(img.at<uint8_t>(2, 1, 0)));
  spdlog::info("G: {}", static_cast<int>(img.at<uint8_t>(2, 1, 1)));
  spdlog::info("B: {}", static_cast<int>(img.at<uint8_t>(2, 1, 2)));

  // 方法2: Vec访问
  spdlog::info("方法2 - Vec访问像素(1,2):");
  auto pixel = img.at3(2, 1); // 自动推导为Vec3b
  spdlog::info("RGB: ({}, {}, {})", static_cast<int>(pixel[0]),
               static_cast<int>(pixel[1]), static_cast<int>(pixel[2]));

  // 方法3: 指针访问
  spdlog::info("方法3 - 指针访问像素(1,2):");
  uint8_t* row_ptr = img.ptr<uint8_t>(2);
  int pixel_offset = 1 * img.channels();
  spdlog::info("R: {}", static_cast<int>(row_ptr[pixel_offset + 0]));
  spdlog::info("G: {}", static_cast<int>(row_ptr[pixel_offset + 1]));
  spdlog::info("B: {}", static_cast<int>(row_ptr[pixel_offset + 2]));

  // 验证内存地址计算
  spdlog::info("内存地址计算验证:");
  size_t byte_offset = 2 * img.matrix().step() + 1 * img.channels();
  const uint8_t* base_ptr = img.ptr<uint8_t>(0);
  const uint8_t* target_ptr = base_ptr + byte_offset;
  spdlog::info("基地址: {}", reinterpret_cast<uintptr_t>(base_ptr));
  spdlog::info("目标地址: {}", reinterpret_cast<uintptr_t>(target_ptr));
  spdlog::info("字节偏移: {}", byte_offset);
  spdlog::info("像素值: {}", static_cast<int>(*target_ptr));
}

// 演示不同数据类型的内存布局
void demonstrate_data_types() {
  spdlog::info("=== 不同数据类型内存布局对比 ===");

  // 创建相同尺寸的不同类型图像
  Image img_uint8(2, 2, 3, DataType::UINT8);
  Image img_float(2, 2, 3, DataType::FLOAT32);

  print_memory_layout(img_uint8, "UINT8图像");
  print_memory_layout(img_float, "FLOAT32图像");

  // 计算内存使用量
  size_t uint8_size = img_uint8.matrix().total() * img_uint8.matrix().elemSize();
  size_t float_size = img_float.matrix().total() * img_float.matrix().elemSize();

  spdlog::info("内存使用对比:");
  spdlog::info("UINT8: {} 字节", uint8_size);
  spdlog::info("FLOAT32: {} 字节", float_size);
  spdlog::info("比例: {:.2f}x", static_cast<double>(float_size) / uint8_size);
}

// 演示ROI内存布局
void demonstrate_roi() {
  spdlog::info("=== ROI内存布局演示 ===");

  // 创建原始图像
  Image img(6, 4, 3);
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      img.at<uint8_t>(y, x, 0) = y * 10 + x;      // R
      img.at<uint8_t>(y, x, 1) = y * 10 + x + 10; // G
      img.at<uint8_t>(y, x, 2) = y * 10 + x + 20; // B
    }
  }

  print_memory_layout(img, "原始6×4图像");

  // 提取ROI
  Rect roi_region(2, 1, 3, 2);  // x=2,y=1,width=3,height=2
  Image roi_img = img.pixel_roi(roi_region);

  print_memory_layout(roi_img, "ROI图像(2,1,3,2)");

  // 验证ROI数据
  spdlog::info("ROI数据验证:");
  spdlog::info("ROI左上角像素(0,0)应该对应原图像像素(2,1):");
  auto roi_pixel = roi_img.at3(0, 0);
  auto orig_pixel = img.at3(1, 2);
  spdlog::info("ROI: ({}, {}, {})", static_cast<int>(roi_pixel[0]),
               static_cast<int>(roi_pixel[1]), static_cast<int>(roi_pixel[2]));
  spdlog::info("原图: ({}, {}, {})", static_cast<int>(orig_pixel[0]),
               static_cast<int>(orig_pixel[1]),
               static_cast<int>(orig_pixel[2]));
}

// 性能测试：不同访问方式的性能
void performance_test() {
  spdlog::info("=== 访问性能测试 ===");

  const int width = 1000;
  const int height = 1000;
  Image img(width, height, 3);

  // 方法1: at()访问
  auto start = std::chrono::high_resolution_clock::now();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      img.at<uint8_t>(y, x, 0) = (y + x) % 256;
      img.at<uint8_t>(y, x, 1) = (y + x + 1) % 256;
      img.at<uint8_t>(y, x, 2) = (y + x + 2) % 256;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // 方法2: 指针访问
  start = std::chrono::high_resolution_clock::now();
  for (int y = 0; y < height; ++y) {
    uint8_t* row_ptr = img.ptr<uint8_t>(y);
    for (int x = 0; x < width; ++x) {
      int offset = x * 3;
      row_ptr[offset + 0] = (y + x) % 256;
      row_ptr[offset + 1] = (y + x + 1) % 256;
      row_ptr[offset + 2] = (y + x + 2) % 256;
    }
  }
  end = std::chrono::high_resolution_clock::now();
  auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  spdlog::info("at()访问时间: {} ms", time1.count());
  spdlog::info("指针访问时间: {} ms", time2.count());
  spdlog::info("性能提升: {:.2f}x",
               static_cast<double>(time1.count()) / time2.count());
}

} // namespace test_memory_layout
} // namespace dip

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("=== dip::Image 内存布局演示程序 ===");

  try {
    dip::test_memory_layout::demonstrate_pixel_access();
    dip::test_memory_layout::demonstrate_data_types();
    dip::test_memory_layout::demonstrate_roi();
    dip::test_memory_layout::performance_test();

    spdlog::info("=== 演示完成 ===");

  } catch (const std::exception& e) {
    spdlog::error("错误: {}", e.what());
    return 1;
  }

  return 0;
}
