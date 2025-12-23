#ifndef ALGORITHMS_CONNECTIVITY_HPP
#define ALGORITHMS_CONNECTIVITY_HPP

#include "core/basic_types.hpp"
#include "core/image.hpp"
#include <vector>

namespace dip {

// 邻域类型枚举
enum class NeighborhoodType {
  N4 = 4,           // 4-邻域
  N8 = 8,           // 8-邻域
  M_CONNECTED = 'm' // m-连通邻域
};

/**
 * 获取像素的指定邻域
 * @param img 输入图像
 * @param x 像素x坐标
 * @param y 像素y坐标
 * @param type 邻域类型 (4, 8, 或 'm')
 * @return 邻域像素坐标列表
 */
std::vector<Point2i> get_neighbors(const Image &img, int x, int y,
                                   NeighborhoodType type);

/**
 * 判断两个像素是否连通
 * @param img 输入图像
 * @param x1 第一个像素的x坐标
 * @param y1 第一个像素的y坐标
 * @param x2 第二个像素的x坐标
 * @param y2 第二个像素的y坐标
 * @param type 邻域类型
 * @param V 连通性判断的灰度值
 * @return 是否连通
 */
bool is_connected(const Image &img, int x1, int y1, int x2, int y2,
                  NeighborhoodType type, uint8_t V);

} // namespace dip

#endif // ALGORITHMS_CONNECTIVITY_HPP
