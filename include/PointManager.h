#ifndef POINT_MANAGER_H
#define POINT_MANAGER_H

#include <string>

#include "GridIndex.h"
#include "Point.h"

class PointManager {
 private:
  GridIndex gridIndex;        // 网格索引
  std::vector<Point> points;  // 存储所有点数据

 public:
  // 构造函数，初始化网格索引
  PointManager(double minLon, double maxLon, double minLat, double maxLat,
               double cellSize);

  // 从文件加载点数据
  void loadFromFile(const std::string &filename);

  // 保存点数据到文件
  void saveToFile(const std::string &filename) const;

  // 插入点
  void insertPoint(const Point &point);

  // 删除点
  void removePoint(const Point &point);

  // 查找点
  bool findPoint(const Point &point) const;

  // 范围查询
  std::vector<Point> rangeQuery(double minLon, double maxLon, double minLat,
                                double maxLat) const;
};

#endif  // POINT_MANAGER_H
