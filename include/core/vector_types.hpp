#ifndef CORE_VECTOR_TYPES_HPP
#define CORE_VECTOR_TYPES_HPP

#include "basic_types.hpp"
#include <algorithm>
#include <cmath>
#include <initializer_list>

namespace dip {

// 固定大小向量（像素值）
template <typename T, int N> class Vec {
public:
  T data[N];

  // 默认构造
  Vec() {
    for (int i = 0; i < N; ++i)
      data[i] = T(0);
  }

  // 列表初始化
  Vec(const std::initializer_list<T> &list) {
    int i = 0;
    for (const auto &val : list) {
      if (i < N)
        data[i++] = val;
    }
    // 未指定的元素保持为0
  }

  // 可变参数构造
  template <typename... Args> Vec(Args... args) : data{T(args)...} {
    static_assert(sizeof...(args) <= N, "Too many arguments");
  }

  // 拷贝构造
  Vec(const Vec &other) {
    for (int i = 0; i < N; ++i)
      data[i] = other.data[i];
  }

  // 类型转换构造
  template <typename T2> Vec(const Vec<T2, N> &other) {
    for (int i = 0; i < N; ++i)
      data[i] = static_cast<T>(other[i]);
  }

  // 赋值操作符
  Vec &operator=(const Vec &other) {
    if (this != &other) {
      for (int i = 0; i < N; ++i)
        data[i] = other.data[i];
    }
    return *this;
  }

  template <typename T2> Vec &operator=(const Vec<T2, N> &other) {
    for (int i = 0; i < N; ++i)
      data[i] = static_cast<T>(other[i]);
    return *this;
  }

  // 索引访问
  T &operator[](int i) { return data[i]; }
  const T &operator[](int i) const { return data[i]; }

  T &operator()(int i) { return data[i]; }
  const T &operator()(int i) const { return data[i]; }

  // 向量运算
  Vec operator+(const Vec &other) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] + other[i];
    return result;
  }

  Vec operator-(const Vec &other) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] - other[i];
    return result;
  }

  Vec operator*(T scale) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] * scale;
    return result;
  }

  Vec operator/(T scale) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] / scale;
    return result;
  }

  // 逐元素乘法
  Vec mul(const Vec &other) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] * other[i];
    return result;
  }

  // 逐元素除法
  Vec div(const Vec &other) const {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = data[i] / other[i];
    return result;
  }

  // 复合运算
  Vec &operator+=(const Vec &other) {
    for (int i = 0; i < N; ++i)
      data[i] += other[i];
    return *this;
  }

  Vec &operator-=(const Vec &other) {
    for (int i = 0; i < N; ++i)
      data[i] -= other[i];
    return *this;
  }

  Vec &operator*=(T scale) {
    for (int i = 0; i < N; ++i)
      data[i] *= scale;
    return *this;
  }

  Vec &operator/=(T scale) {
    for (int i = 0; i < N; ++i)
      data[i] /= scale;
    return *this;
  }

  // 比较运算
  bool operator==(const Vec &other) const {
    for (int i = 0; i < N; ++i)
      if (data[i] != other[i])
        return false;
    return true;
  }

  bool operator!=(const Vec &other) const { return !(*this == other); }

  // 数学运算
  T dot(const Vec &other) const {
    T result = T(0);
    for (int i = 0; i < N; ++i)
      result += data[i] * other[i];
    return result;
  }

  T length() const { return std::sqrt(dot(*this)); }
  T length_squared() const { return dot(*this); }

  Vec normalized() const {
    T len = length();
    return len > T(0) ? (*this) / len : Vec();
  }

  void normalize() {
    T len = length();
    if (len > T(0))
      *this /= len;
  }

  // 范数
  template <int P = 2> T norm() const {
    if constexpr (P == 1) {
      T result = T(0);
      for (int i = 0; i < N; ++i)
        result += std::abs(data[i]);
      return result;
    } else if constexpr (P == 2) {
      return length();
    } else if constexpr (P == 0) { // L-infinity
      T result = T(0);
      for (int i = 0; i < N; ++i)
        result = std::max(result, std::abs(data[i]));
      return result;
    }
  }

  // 最小/最大值
  T min() const {
    T result = data[0];
    for (int i = 1; i < N; ++i)
      result = std::min(result, data[i]);
    return result;
  }

  T max() const {
    T result = data[0];
    for (int i = 1; i < N; ++i)
      result = std::max(result, data[i]);
    return result;
  }

  int argmin() const {
    int idx = 0;
    for (int i = 1; i < N; ++i) {
      if (data[i] < data[idx])
        idx = i;
    }
    return idx;
  }

  int argmax() const {
    int idx = 0;
    for (int i = 1; i < N; ++i) {
      if (data[i] > data[idx])
        idx = i;
    }
    return idx;
  }

  // 获取子向量
  template <int M> Vec<T, M> subvec(int start = 0) const {
    Vec<T, M> result;
    for (int i = 0; i < M && (start + i) < N; ++i) {
      result[i] = data[start + i];
    }
    return result;
  }

  // 静态工厂方法
  static Vec zeros() { return Vec(); }
  static Vec ones() {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = T(1);
    return result;
  }

  static Vec fill(T value) {
    Vec result;
    for (int i = 0; i < N; ++i)
      result[i] = value;
    return result;
  }

  // 常用向量
  static Vec unitX() {
    Vec result;
    if (N > 0)
      result[0] = T(1);
    return result;
  }

  static Vec unitY() {
    Vec result;
    if (N > 1)
      result[1] = T(1);
    return result;
  }

  static Vec unitZ() {
    Vec result;
    if (N > 2)
      result[2] = T(1);
    return result;
  }
};

// 标量（用于颜色等）
class Scalar {
public:
  double val[4];

  Scalar() { val[0] = val[1] = val[2] = val[3] = 0; }
  Scalar(double v0) {
    val[0] = v0;
    val[1] = val[2] = val[3] = 0;
  }
  Scalar(double v0, double v1) {
    val[0] = v0;
    val[1] = v1;
    val[2] = val[3] = 0;
  }
  Scalar(double v0, double v1, double v2) {
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
    val[3] = 0;
  }
  Scalar(double v0, double v1, double v2, double v3) {
    val[0] = v0;
    val[1] = v1;
    val[2] = v2;
    val[3] = v3;
  }

  template <typename T, int N> Scalar(const Vec<T, N> &vec) {
    for (int i = 0; i < 4; ++i)
      val[i] = (i < N) ? static_cast<double>(vec[i]) : 0;
  }

  double &operator[](int i) { return val[i]; }
  const double &operator[](int i) const { return val[i]; }

  // 转换为Vec
  template <int N> Vec<double, N> toVec() const {
    Vec<double, N> result;
    for (int i = 0; i < N && i < 4; ++i) {
      result[i] = val[i];
    }
    return result;
  }

  bool operator==(const Scalar &other) const {
    for (int i = 0; i < 4; ++i) {
      if (std::abs(val[i] - other[i]) > 1e-10)
        return false;
    }
    return true;
  }

  bool operator!=(const Scalar &other) const { return !(*this == other); }

  Scalar operator+(const Scalar &other) const {
    Scalar result;
    for (int i = 0; i < 4; ++i)
      result[i] = val[i] + other[i];
    return result;
  }

  Scalar operator-(const Scalar &other) const {
    Scalar result;
    for (int i = 0; i < 4; ++i)
      result[i] = val[i] - other[i];
    return result;
  }

  Scalar operator*(double scale) const {
    Scalar result;
    for (int i = 0; i < 4; ++i)
      result[i] = val[i] * scale;
    return result;
  }

  friend std::ostream &operator<<(std::ostream &os, const Scalar &s) {
    return os << "[" << s[0] << ", " << s[1] << ", " << s[2] << ", " << s[3]
              << "]";
  }
};

// 全局运算符
template <typename T, int N>
Vec<T, N> operator*(T scale, const Vec<T, N> &vec) {
  return vec * scale;
}

// 常用向量类型别名
using Vec2b = Vec<uint8_t, 2>;
using Vec3b = Vec<uint8_t, 3>;
using Vec4b = Vec<uint8_t, 4>;

using Vec2s = Vec<int16_t, 2>;
using Vec3s = Vec<int16_t, 3>;
using Vec4s = Vec<int16_t, 4>;

using Vec2i = Vec<int32_t, 2>;
using Vec3i = Vec<int32_t, 3>;
using Vec4i = Vec<int32_t, 4>;

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

using Vec2d = Vec<double, 2>;
using Vec3d = Vec<double, 3>;
using Vec4d = Vec<double, 4>;

} // namespace dip

#endif // CORE_VECTOR_TYPES_HPP
