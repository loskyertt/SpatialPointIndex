#ifndef GRID_INDEX_H
#define GRID_INDEX_H

#include <unordered_map>
#include <vector>

#include "Point.h"

// 网格单元定义
struct GridCell {
  std::vector<Point> points;  // 存储在该网格单元中的点
};

class GridIndex {
 public:
  // 构造函数，指定网格的范围和每个单元的大小
  GridIndex(double minLon, double maxLon, double minLat, double maxLat,
            double cellSize);

  // 插入点
  void insert(const Point &point);

  // 删除点
  void remove(const Point &point);

  // 查找点（返回是否存在）
  bool find(const Point &point) const;

  // 范围查询，返回在指定矩形范围内的点
  std::vector<Point> rangeQuery(double minLon, double maxLon, double minLat,
                                double maxLat) const;

 private:
  // 根据经纬度计算网格单元的索引
  std::pair<int, int> getCellIndex(double longitude, double latitude) const;

  // 网格范围
  double minLon, maxLon, minLat, maxLat;

  // 网格单元大小
  double cellSize;

  // 存储所有网格单元的哈希表
  std::unordered_map<int, std::unordered_map<int, GridCell>> grid;
};

#endif  // GRID_INDEX_H
