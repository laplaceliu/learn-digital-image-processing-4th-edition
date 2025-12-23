#include "algorithms/connectivity.hpp"
#include "core/core.hpp"
#include <spdlog/spdlog.h>

namespace dip {

std::vector<Point2i> get_neighbors(const Image &img, int x, int y,
                                   NeighborhoodType type) {
  std::vector<Point2i> neighbors;

  int height = img.height();
  int width = img.width();

  // 4-邻域定义
  std::vector<Point2i> n4 = {{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}};

  // 对角邻域定义
  std::vector<Point2i> nd = {
      {x + 1, y + 1}, {x + 1, y - 1}, {x - 1, y + 1}, {x - 1, y - 1}};

  if (type == NeighborhoodType::N4) {
    // 4-邻域
    for (const auto &p : n4) {
      if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
        neighbors.push_back(p);
      }
    }
  } else if (type == NeighborhoodType::N8) {
    // 8-邻域 = 4-邻域 + 对角邻域
    for (const auto &p : n4) {
      if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
        neighbors.push_back(p);
      }
    }
    for (const auto &p : nd) {
      if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
        neighbors.push_back(p);
      }
    }
  } else if (type == NeighborhoodType::M_CONNECTED) {
    // m-连通邻域
    // 条件1：4-邻域总是包含
    for (const auto &p : n4) {
      if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
        neighbors.push_back(p);
      }
    }

    // 条件2：对角邻域需要满足额外条件
    // 对于m-连通，两个对角像素p和q是m连通的当且仅当：
    // 1) p和q的V值相等
    // 2) 不存在值为V的像素在p和q之间的4-邻域路径上

    for (const auto &q : nd) {
      if (q.x < 0 || q.x >= width || q.y < 0 || q.y >= height) {
        continue;
      }

      // 找到与对角像素q相邻的两个4-邻域像素
      std::vector<Point2i> path_intermediates;
      if (q.x > x && q.y > y) { // 右下
        path_intermediates = {{x + 1, y}, {x, y + 1}};
      } else if (q.x > x && q.y < y) { // 右上
        path_intermediates = {{x + 1, y}, {x, y - 1}};
      } else if (q.x < x && q.y > y) { // 左下
        path_intermediates = {{x - 1, y}, {x, y + 1}};
      } else if (q.x < x && q.y < y) { // 左上
        path_intermediates = {{x - 1, y}, {x, y - 1}};
      }

      // 检查这两个中间像素是否都不为V值
      bool can_add = true;
      for (const auto &p : path_intermediates) {
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
          uint8_t val = img.get_pixel(p.x, p.y, 0);
          if (val ==
              img.get_pixel(x, y, 0)) { // 如果中间像素的值等于当前像素的值
            can_add = false;
            break;
          }
        }
      }

      if (can_add) {
        neighbors.push_back(q);
      }
    }
  }

  return neighbors;
}

bool is_connected(const Image &img, int x1, int y1, int x2, int y2,
                  NeighborhoodType type, uint8_t V) {
  // 检查坐标是否在图像范围内
  int height = img.height();
  int width = img.width();

  if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height || x2 < 0 ||
      x2 >= width || y2 < 0 || y2 >= height) {
    return false;
  }

  // 检查两个像素的灰度值是否等于V
  uint8_t val1 = img.get_pixel(x1, y1, 0);
  uint8_t val2 = img.get_pixel(x2, y2, 0);

  if (val1 != V || val2 != V) {
    return false;
  }

  // 获取第一个像素的邻域
  std::vector<Point2i> neighbors = get_neighbors(img, x1, y1, type);

  // 检查第二个像素是否在第一个像素的邻域中
  for (const auto &p : neighbors) {
    if (p.x == x2 && p.y == y2) {
      return true;
    }
  }

  return false;
}

} // namespace dip
