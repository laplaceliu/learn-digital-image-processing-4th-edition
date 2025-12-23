#include <algorithms/connectivity.hpp>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  spdlog::info("=== m-连通性详细测试 ===");

  // 创建一个测试图像来展示m-连通和8-连通的区别
  dip::Image test_img(3, 3, 1, dip::DataType::UINT8);
  test_img.setTo(dip::Scalar(0));

  // 设置一个十字形图案：只有中心点和上下左右为1，对角线为0
  test_img.set_pixel(1, 1, 0, 1); // 中心
  test_img.set_pixel(1, 0, 0, 1); // 上
  test_img.set_pixel(2, 1, 0, 1); // 右
  test_img.set_pixel(1, 2, 0, 1); // 下
  test_img.set_pixel(0, 1, 0, 1); // 左

  spdlog::info("测试图像 (3x3):");
  spdlog::info("  0 1 2 (x)");
  spdlog::info("0 0 1 0");
  spdlog::info("1 1 1 1");
  spdlog::info("2 0 1 0");
  spdlog::info("(y)");

  // 测试中心点的邻域
  spdlog::info("中心点(1,1)的邻域分析:");

  // 4-邻域
  auto neighbors_4 =
      dip::get_neighbors(test_img, 1, 1, dip::NeighborhoodType::N4);
  std::string result_4 = "4-邻域: ";
  for (const auto &p : neighbors_4) {
    result_4 += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_4);

  // 8-邻域
  auto neighbors_8 =
      dip::get_neighbors(test_img, 1, 1, dip::NeighborhoodType::N8);
  std::string result_8 = "8-邻域: ";
  for (const auto &p : neighbors_8) {
    result_8 += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_8);

  // m-连通邻域
  auto neighbors_m =
      dip::get_neighbors(test_img, 1, 1, dip::NeighborhoodType::M_CONNECTED);
  std::string result_m = "m-连通邻域: ";
  for (const auto &p : neighbors_m) {
    result_m += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_m);

  // 测试连通性
  spdlog::info("连通性测试 (V=1):");

  // 测试对角连通性
  bool conn_8_diag =
      dip::is_connected(test_img, 1, 0, 0, 1, dip::NeighborhoodType::N8, 1);
  bool conn_m_diag = dip::is_connected(test_img, 1, 0, 0, 1,
                                       dip::NeighborhoodType::M_CONNECTED, 1);
  spdlog::info("(1,0) -> (0,1) 对角连通性:");
  spdlog::info("  8-邻域: {}", conn_8_diag ? "true" : "false");
  spdlog::info("  m-连通: {}", conn_m_diag ? "true" : "false");

  // 测试4-邻域连通性
  bool conn_4 =
      dip::is_connected(test_img, 1, 1, 1, 0, dip::NeighborhoodType::N4, 1);
  spdlog::info("(1,1) -> (1,0) 4-邻域连通性: {}", conn_4 ? "true" : "false");

  // 现在创建另一个测试：移除中间像素，展示m-连通如何包含对角
  dip::Image test_img2(3, 3, 1, dip::DataType::UINT8);
  test_img2.setTo(dip::Scalar(0));

  // 只有对角线为1，中间为0
  test_img2.set_pixel(0, 0, 0, 1);
  test_img2.set_pixel(2, 0, 0, 1);
  test_img2.set_pixel(0, 2, 0, 1);
  test_img2.set_pixel(2, 2, 0, 1);

  spdlog::info("=== 对角线测试 ===");
  spdlog::info("测试图像 (只有对角线为1):");
  spdlog::info("1 0 1");
  spdlog::info("0 0 0");
  spdlog::info("1 0 1");

  auto neighbors_m2 =
      dip::get_neighbors(test_img2, 1, 1, dip::NeighborhoodType::M_CONNECTED);
  std::string result_m2 = "中心(1,1)的m-连通邻域: ";
  for (const auto &p : neighbors_m2) {
    result_m2 += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ") ";
  }
  spdlog::info(result_m2);

  // 测试对角连通性
  bool conn_m_diag2 = dip::is_connected(test_img2, 0, 0, 2, 2,
                                        dip::NeighborhoodType::M_CONNECTED, 0);
  spdlog::info("(0,0) -> (2,2) m-连通性 (V=0): {}",
               conn_m_diag2 ? "true" : "false");

  spdlog::info("m-连通性详细测试完成！");
  return 0;
}
