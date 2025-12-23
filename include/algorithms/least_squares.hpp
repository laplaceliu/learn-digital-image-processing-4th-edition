#ifndef ALGORITHMS_LEAST_SQUARES_HPP
#define ALGORITHMS_LEAST_SQUARES_HPP

#include <core/core.hpp>
#include <vector>

namespace dip {
namespace algorithms {

// 最小二乘法线性拟合
// 用于拟合数据点 (x_i, y_i) 到直线 y = ax + b
struct LinearFitResult {
  double slope;     // 斜率 a
  double intercept; // 截距 b
  double r_squared; // 决定系数 R²
  bool success;     // 拟合是否成功
};

// 线性拟合（最小二乘法）
// @param x_data x坐标数据点
// @param y_data y坐标数据点
// @return 拟合结果，包含斜率、截距、决定系数和成功标志
LinearFitResult linear_fit(const std::vector<double> &x_data,
                           const std::vector<double> &y_data);

// 多项式拟合（最小二乘法）
// @param x_data x坐标数据点
// @param y_data y坐标数据点
// @param degree 多项式次数
// @return 拟合系数（从常数项到最高次项），空向量表示失败
std::vector<double> polynomial_fit(const std::vector<double> &x_data,
                                   const std::vector<double> &y_data,
                                   int degree);

// 计算R²决定系数
// @param y_true 真实值
// @param y_pred 预测值
// @return R²值（0-1之间，越接近1拟合越好）
double calculate_r_squared(const std::vector<double> &y_true,
                           const std::vector<double> &y_pred);

// 使用拟合模型进行预测
// @param x 输入值
// @param coefficients 拟合系数（线性拟合：[b, a]，多项式拟合：[a0, a1, ...,
// an]）
// @return 预测值
double predict(double x, const std::vector<double> &coefficients);

} // namespace algorithms
} // namespace dip

#endif // ALGORITHMS_LEAST_SQUARES_HPP
