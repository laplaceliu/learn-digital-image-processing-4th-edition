#include <algorithms/least_squares.hpp>
#include <core/core.hpp>
#include <iostream>
#include <random>
#include <spdlog/spdlog.h>
#include <vector>

using namespace dip;

int main() {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("Least Squares Algorithm Test");

  // 生成测试数据：y = 2.5x + 10 + noise
  std::vector<double> x_data, y_data;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<> noise_dist(0.0, 5.0); // 标准差为5的噪声

  for (double x = 0.0; x <= 10.0; x += 0.5) {
    x_data.push_back(x);
    double y = 2.5 * x + 10.0 + noise_dist(gen);
    y_data.push_back(y);
  }

  spdlog::info("Generated {} data points", x_data.size());

  // 测试线性拟合
  spdlog::info("\n--- Linear Fit Test ---");
  auto linear_result = algorithms::linear_fit(x_data, y_data);

  if (linear_result.success) {
    spdlog::info("Linear fit: y = {:.3f}x + {:.3f}", linear_result.slope,
                 linear_result.intercept);
    spdlog::info("R² = {:.6f}", linear_result.r_squared);

    // 测试预测
    double test_x = 5.0;
    std::vector<double> linear_coeffs = {linear_result.intercept,
                                         linear_result.slope};
    double predicted = algorithms::predict(test_x, linear_coeffs);
    double expected = 2.5 * test_x + 10.0;
    spdlog::info("At x = {:.1f}: predicted = {:.3f}, expected ≈ {:.3f}", test_x,
                 predicted, expected);
  } else {
    spdlog::error("Linear fit failed");
  }

  // 测试二次多项式拟合
  spdlog::info("\n--- Quadratic Polynomial Fit Test ---");
  auto poly_coeffs = algorithms::polynomial_fit(x_data, y_data, 2);

  if (!poly_coeffs.empty()) {
    spdlog::info("Quadratic fit: y = {:.3f}x² + {:.3f}x + {:.3f}",
                 poly_coeffs[2], poly_coeffs[1], poly_coeffs[0]);

    // 计算R²
    std::vector<double> y_pred(x_data.size());
    for (size_t i = 0; i < x_data.size(); ++i) {
      y_pred[i] = algorithms::predict(x_data[i], poly_coeffs);
    }
    double r_squared = algorithms::calculate_r_squared(y_data, y_pred);
    spdlog::info("R² = {:.6f}", r_squared);

    // 测试预测
    double test_x = 5.0;
    double predicted = algorithms::predict(test_x, poly_coeffs);
    spdlog::info("At x = {:.1f}: predicted = {:.3f}", test_x, predicted);
  } else {
    spdlog::error("Quadratic fit failed");
  }

  // 测试三次多项式拟合
  spdlog::info("\n--- Cubic Polynomial Fit Test ---");
  auto cubic_coeffs = algorithms::polynomial_fit(x_data, y_data, 3);

  if (!cubic_coeffs.empty()) {
    spdlog::info("Cubic fit: y = {:.3f}x³ + {:.3f}x² + {:.3f}x + {:.3f}",
                 cubic_coeffs[3], cubic_coeffs[2], cubic_coeffs[1],
                 cubic_coeffs[0]);

    // 计算R²
    std::vector<double> y_pred(x_data.size());
    for (size_t i = 0; i < x_data.size(); ++i) {
      y_pred[i] = algorithms::predict(x_data[i], cubic_coeffs);
    }
    double r_squared = algorithms::calculate_r_squared(y_data, y_pred);
    spdlog::info("R² = {:.6f}", r_squared);
  } else {
    spdlog::error("Cubic fit failed");
  }

  // 边界情况测试
  spdlog::info("\n--- Edge Cases Test ---");

  // 测试数据点不足的情况
  std::vector<double> few_x = {1.0}, few_y = {2.0};
  auto edge_linear = algorithms::linear_fit(few_x, few_y);
  spdlog::info("Linear fit with 1 point: success = {}", edge_linear.success);

  // 测试数据不匹配的情况
  auto edge_linear2 = algorithms::linear_fit(x_data, {1.0, 2.0});
  spdlog::info("Linear fit with mismatched data: success = {}",
               edge_linear2.success);

  // 测试垂直线情况
  std::vector<double> vertical_x = {1.0, 1.0, 1.0};
  std::vector<double> vertical_y = {1.0, 2.0, 3.0};
  auto vertical_result = algorithms::linear_fit(vertical_x, vertical_y);
  spdlog::info("Linear fit with vertical line: success = {}",
               vertical_result.success);

  spdlog::info("\nLeast squares test completed!");
  return 0;
}
