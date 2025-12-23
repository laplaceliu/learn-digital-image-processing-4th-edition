#ifndef CORE_BASIC_TYPES_HPP
#define CORE_BASIC_TYPES_HPP

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

// 图像数据类型枚举
enum class DataType {
  UINT8,   // 8-bit unsigned char
  INT8,    // 8-bit signed char
  UINT16,  // 16-bit unsigned short
  INT16,   // 16-bit signed short
  FLOAT32, // 32-bit float
  FLOAT64  // 64-bit double
};

// 像素大小（字节）
inline int dataTypeSize(DataType type) {
  switch (type) {
  case DataType::UINT8:
  case DataType::INT8:
    return 1;
  case DataType::UINT16:
  case DataType::INT16:
    return 2;
  case DataType::FLOAT32:
    return 4;
  case DataType::FLOAT64:
    return 8;
  default:
    return 0;
  }
}

// 数据类型名称
inline std::string dataTypeName(DataType type) {
  switch (type) {
  case DataType::UINT8:
    return "UINT8";
  case DataType::INT8:
    return "INT8";
  case DataType::UINT16:
    return "UINT16";
  case DataType::INT16:
    return "INT16";
  case DataType::FLOAT32:
    return "FLOAT32";
  case DataType::FLOAT64:
    return "FLOAT64";
  default:
    return "UNKNOWN";
  }
}

// 2D尺寸
struct Size {
  int width;
  int height;

  Size() : width(0), height(0) {}
  Size(int w, int h) : width(w), height(h) {}

  bool empty() const { return width <= 0 || height <= 0; }
  size_t area() const { return static_cast<size_t>(width) * height; }

  bool operator==(const Size &other) const {
    return width == other.width && height == other.height;
  }
  bool operator!=(const Size &other) const { return !(*this == other); }

  friend std::ostream &operator<<(std::ostream &os, const Size &size) {
    return os << size.width << "x" << size.height;
  }
};

// 2D点坐标
template <typename T> struct Point {
  T x, y;

  Point() : x(0), y(0) {}
  Point(T x_, T y_) : x(x_), y(y_) {}

  template <typename T2>
  Point(const Point<T2> &other) : x(other.x), y(other.y) {}

  Point operator+(const Point &other) const {
    return Point(x + other.x, y + other.y);
  }
  Point operator-(const Point &other) const {
    return Point(x - other.x, y - other.y);
  }
  Point operator*(T scale) const { return Point(x * scale, y * scale); }
  Point operator/(T scale) const { return Point(x / scale, y / scale); }

  Point &operator+=(const Point &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Point &operator-=(const Point &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Point &operator*=(T scale) {
    x *= scale;
    y *= scale;
    return *this;
  }
  Point &operator/=(T scale) {
    x /= scale;
    y /= scale;
    return *this;
  }

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Point &other) const { return !(*this == other); }

  template <typename T2> bool operator<(const Point<T2> &other) const {
    return y < other.y || (y == other.y && x < other.x);
  }

  friend std::ostream &operator<<(std::ostream &os, const Point &pt) {
    return os << "(" << pt.x << ", " << pt.y << ")";
  }
};

using Point2i = Point<int>;
using Point2f = Point<float>;
using Point2d = Point<double>;

// 矩形区域
struct Rect {
  int x, y, width, height;

  Rect() : x(0), y(0), width(0), height(0) {}
  Rect(int x_, int y_, int w_, int h_) : x(x_), y(y_), width(w_), height(h_) {}
  Rect(const Point2i &pt, const Size &sz)
      : x(pt.x), y(pt.y), width(sz.width), height(sz.height) {}
  Rect(const Point2i &tl, const Point2i &br)
      : x(tl.x), y(tl.y), width(br.x - tl.x), height(br.y - tl.y) {}

  Point2i tl() const { return Point2i(x, y); }                  // top-left
  Point2i tr() const { return Point2i(x + width, y); }          // top-right
  Point2i bl() const { return Point2i(x, y + height); }         // bottom-left
  Point2i br() const { return Point2i(x + width, y + height); } // bottom-right
  Point2i center() const { return Point2i(x + width / 2, y + height / 2); }
  Size size() const { return Size(width, height); }
  int area() const { return width * height; }

  bool contains(const Point2i &pt) const {
    return pt.x >= x && pt.x < x + width && pt.y >= y && pt.y < y + height;
  }

  bool contains(const Rect &other) const {
    return contains(other.tl()) && contains(other.br());
  }

  bool empty() const { return width <= 0 || height <= 0; }

  // 求交集
  Rect operator&(const Rect &other) const {
    int x1 = std::max(x, other.x);
    int y1 = std::max(y, other.y);
    int x2 = std::min(x + width, other.x + other.width);
    int y2 = std::min(y + height, other.y + other.height);

    if (x2 <= x1 || y2 <= y1)
      return Rect(); // 无交集
    return Rect(x1, y1, x2 - x1, y2 - y1);
  }

  // 求并集
  Rect operator|(const Rect &other) const {
    if (empty())
      return other;
    if (other.empty())
      return *this;

    int x1 = std::min(x, other.x);
    int y1 = std::min(y, other.y);
    int x2 = std::max(x + width, other.x + other.width);
    int y2 = std::max(y + height, other.y + other.height);

    return Rect(x1, y1, x2 - x1, y2 - y1);
  }

  bool operator==(const Rect &other) const {
    return x == other.x && y == other.y && width == other.width &&
           height == other.height;
  }

  bool operator!=(const Rect &other) const { return !(*this == other); }

  // 扩展矩形
  Rect inflate(int dx, int dy) const {
    return Rect(x - dx, y - dy, width + 2 * dx, height + 2 * dy);
  }

  Rect inflate(int d) const { return inflate(d, d); }

  // 平移矩形
  Rect translate(int dx, int dy) const {
    return Rect(x + dx, y + dy, width, height);
  }

  friend std::ostream &operator<<(std::ostream &os, const Rect &rect) {
    return os << "Rect(" << rect.x << ", " << rect.y << ", " << rect.width
              << ", " << rect.height << ")";
  }
};

#endif // CORE_BASIC_TYPES_HPP
