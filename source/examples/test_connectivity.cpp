#include <algorithms/connectivity.hpp>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  // 创建一个简单的测试图像 (5x5)
  dip::Image test_img(5, 5, 1, dip::DataType::UINT8);

  // 设置一些测试像素值为1 (V=1)，其余为0
  test_img.setTo(dip::Scalar(0)); // 全部设为0

  // 设置中心点为1
  test_img.set_pixel(2, 2, 0, 1);

  // 设置一些周围的像素
  test_img.set_pixel(2, 1, 0, 1); // 上
  test_img.set_pixel(3, 2, 0, 1); // 右
  test_img.set_pixel(2, 3, 0, 1); // 下
  test_img.set_pixel(1, 2, 0, 1); // 左

  // 设置对角线像素
  test_img.set_pixel(1, 1, 0, 1); // 左上
  test_img.set_pixel(3, 1, 0, 1); // 右上
  test_img.set_pixel(1, 3, 0, 1); // 左下
  test_img.set_pixel(3, 3, 0, 1); // 右下

  spdlog::info("=== 测试连通性算法 ===");
  spdlog::info("图像: 5x5, 中心点(2,2)和周围像素设为1");

  // 测试get_neighbors函数
  spdlog::info("=== 测试 get_neighbors ===");

  // 测试4-邻域
  auto neighbors_4 =
      dip::get_neighbors(test_img, 2, 2, dip::NeighborhoodType::N4);
  std::string result_4 = "4-邻域 (中心点(2,2)): ";
  for (const auto &p : neighbors_4) {
    result_4 += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_4);

  // 测试8-邻域
  auto neighbors_8 =
      dip::get_neighbors(test_img, 2, 2, dip::NeighborhoodType::N8);
  std::string result_8 = "8-邻域 (中心点(2,2)): ";
  for (const auto &p : neighbors_8) {
    result_8 += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_8);

  // 测试m-连通邻域
  auto neighbors_m =
      dip::get_neighbors(test_img, 2, 2, dip::NeighborhoodType::M_CONNECTED);
  std::string result_m = "m-连通邻域 (中心点(2,2)): ";
  for (const auto &p : neighbors_m) {
    result_m += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_m);

  // 测试is_connected函数
  spdlog::info("=== 测试 is_connected (V=1) ===");

  // 测试4-邻域连通性
  bool connected_4 =
      dip::is_connected(test_img, 2, 2, 3, 2, dip::NeighborhoodType::N4, 1);
  spdlog::info("4-邻域连通性 (2,2) -> (3,2): {}",
               connected_4 ? "true" : "false");

  // 测试8-邻域连通性 (对角)
  bool connected_8 =
      dip::is_connected(test_img, 2, 2, 3, 3, dip::NeighborhoodType::N8, 1);
  spdlog::info("8-邻域连通性 (2,2) -> (3,3): {}",
               connected_8 ? "true" : "false");

  // 测试不连通的情况
  bool not_connected =
      dip::is_connected(test_img, 2, 2, 0, 0, dip::NeighborhoodType::N8, 1);
  spdlog::info("8-邻域连通性 (2,2) -> (0,0) [不同值]: {}",
               not_connected ? "true" : "false");

  // 测试边界情况
  auto edge_neighbors =
      dip::get_neighbors(test_img, 0, 0, dip::NeighborhoodType::N4);
  spdlog::info("=== 边界测试 ===");
  std::string result_edge = "4-邻域 (角点(0,0)): ";
  for (const auto &p : edge_neighbors) {
    result_edge += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_edge);

  spdlog::info("连通性算法测试完成！");
  return 0;
}
