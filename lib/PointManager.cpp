#include <algorithm>
#include <fstream>
#include <stdexcept>

#include "PointManager.h"

// 构造函数，初始化网格索引
PointManager::PointManager(double minLon, double maxLon, double minLat,
                           double maxLat, double cellSize)
    : gridIndex(minLon, maxLon, minLat, maxLat, cellSize) {}

// 从文件加载点数据
void PointManager::loadFromFile(const std::string &filename) {
  std::ifstream infile(filename);
  if (!infile) {
    throw std::runtime_error("Unable to open file");
  }

  points.clear();
  double lon, lat;
  while (infile >> lon >> lat) {
    Point point(lon, lat);
    points.push_back(point);
    gridIndex.insert(point);
  }
  infile.close();
}

// 保存点数据到文件
void PointManager::saveToFile(const std::string &filename) const {
  std::ofstream outfile(filename);
  if (!outfile) {
    throw std::runtime_error("Unable to open file");
  }

  for (const auto &point : points) {
    outfile << point.getLongitude() << "\t" << point.getLatitude() << "\n";
  }
  outfile.close();
}

// 插入点
void PointManager::insertPoint(const Point &point) {
  points.push_back(point);
  gridIndex.insert(point);
}

// 删除点
void PointManager::removePoint(const Point &point) {
  auto it = std::remove(points.begin(), points.end(), point);
  points.erase(it, points.end());
  gridIndex.remove(point);
}

// 查找点
bool PointManager::findPoint(const Point &point) const {
  return gridIndex.find(point);
}

// 范围查询
std::vector<Point> PointManager::rangeQuery(double minLon, double maxLon,
                                            double minLat,
                                            double maxLat) const {
  return gridIndex.rangeQuery(minLon, maxLon, minLat, maxLat);
}
