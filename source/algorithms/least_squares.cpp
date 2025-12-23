#include <algorithm>
#include <algorithms/least_squares.hpp>
#include <cmath>
#include <stdexcept>

namespace dip {
namespace algorithms {

LinearFitResult linear_fit(const std::vector<double> &x_data,
                           const std::vector<double> &y_data) {
  LinearFitResult result{0.0, 0.0, 0.0, false};

  // 检查数据有效性
  if (x_data.size() != y_data.size() || x_data.size() < 2) {
    return result;
  }

  size_t n = x_data.size();

  // 计算所需的总和
  double sum_x = 0.0, sum_y = 0.0;
  double sum_xx = 0.0, sum_xy = 0.0;

  for (size_t i = 0; i < n; ++i) {
    sum_x += x_data[i];
    sum_y += y_data[i];
    sum_xx += x_data[i] * x_data[i];
    sum_xy += x_data[i] * y_data[i];
  }

  // 计算斜率和截距
  double denominator = n * sum_xx - sum_x * sum_x;
  if (std::abs(denominator) < 1e-10) {
    return result; // 垂直线，无法拟合
  }

  result.slope = (n * sum_xy - sum_x * sum_y) / denominator;
  result.intercept = (sum_y - result.slope * sum_x) / n;

  // 计算R²
  std::vector<double> y_pred(n);
  for (size_t i = 0; i < n; ++i) {
    y_pred[i] = result.slope * x_data[i] + result.intercept;
  }

  result.r_squared = calculate_r_squared(y_data, y_pred);
  result.success = true;

  return result;
}

std::vector<double> polynomial_fit(const std::vector<double> &x_data,
                                   const std::vector<double> &y_data,
                                   int degree) {
  std::vector<double> coefficients;

  // 检查数据有效性
  if (x_data.size() != y_data.size() || x_data.size() < degree + 1 ||
      degree < 1) {
    return coefficients; // 返回空向量表示失败
  }

  size_t n = x_data.size();
  size_t m = degree + 1; // 系数数量

  // 构建正规方程 A * coefficients = b
  std::vector<std::vector<double>> A(m, std::vector<double>(m, 0.0));
  std::vector<double> b(m, 0.0);

  // 计算A矩阵和b向量
  for (size_t i = 0; i < n; ++i) {
    double x_pow = 1.0;
    for (int j = 0; j <= degree; ++j) {
      // b[j] += y_data[i] * x_data[i]^j
      b[j] += y_data[i] * x_pow;

      double x_pow_times = x_pow;
      for (int k = 0; k <= degree; ++k) {
        // A[j][k] += x_data[i]^(j+k)
        A[j][k] += x_pow_times;
        x_pow_times *= x_data[i];
      }
      x_pow *= x_data[i];
    }
  }

  // 使用高斯消元法求解线性方程组
  coefficients.resize(m, 0.0);

  // 前向消元
  for (size_t i = 0; i < m; ++i) {
    // 找到主元
    size_t max_row = i;
    for (size_t k = i + 1; k < m; ++k) {
      if (std::abs(A[k][i]) > std::abs(A[max_row][i])) {
        max_row = k;
      }
    }

    // 交换行
    if (max_row != i) {
      std::swap(A[i], A[max_row]);
      std::swap(b[i], b[max_row]);
    }

    // 检查主元
    if (std::abs(A[i][i]) < 1e-10) {
      return std::vector<double>(); // 奇异矩阵，无解
    }

    // 消元
    for (size_t k = i + 1; k < m; ++k) {
      double factor = A[k][i] / A[i][i];
      b[k] -= factor * b[i];
      for (size_t j = i; j < m; ++j) {
        A[k][j] -= factor * A[i][j];
      }
    }
  }

  // 回代求解
  for (int i = m - 1; i >= 0; --i) {
    coefficients[i] = b[i];
    for (size_t j = i + 1; j < m; ++j) {
      coefficients[i] -= A[i][j] * coefficients[j];
    }
    coefficients[i] /= A[i][i];
  }

  return coefficients;
}

double calculate_r_squared(const std::vector<double> &y_true,
                           const std::vector<double> &y_pred) {
  if (y_true.size() != y_pred.size() || y_true.empty()) {
    return 0.0;
  }

  size_t n = y_true.size();

  // 计算y_true的均值
  double y_mean = 0.0;
  for (double y : y_true) {
    y_mean += y;
  }
  y_mean /= n;

  // 计算总平方和(SST)和残差平方和(SSE)
  double sst = 0.0, sse = 0.0;
  for (size_t i = 0; i < n; ++i) {
    double diff_true = y_true[i] - y_mean;
    double diff_pred = y_true[i] - y_pred[i];
    sst += diff_true * diff_true;
    sse += diff_pred * diff_pred;
  }

  return (sst > 1e-10) ? 1.0 - (sse / sst) : 0.0;
}

double predict(double x, const std::vector<double> &coefficients) {
  if (coefficients.empty()) {
    return 0.0;
  }

  double result = 0.0;
  double x_pow = 1.0;

  for (double coeff : coefficients) {
    result += coeff * x_pow;
    x_pow *= x;
  }

  return result;
}

} // namespace algorithms
} // namespace dip
