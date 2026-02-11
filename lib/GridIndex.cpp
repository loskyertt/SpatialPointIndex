#include <algorithm>

#include "GridIndex.h"

// 构造函数，初始化网格范围和单元大小
GridIndex::GridIndex(double minLon, double maxLon, double minLat, double maxLat,
                     double cellSize)
    : minLon(minLon),
      maxLon(maxLon),
      minLat(minLat),
      maxLat(maxLat),
      cellSize(cellSize) {}

// 插入点
void GridIndex::insert(const Point &point) {
  auto cellIndex = getCellIndex(point.getLongitude(), point.getLatitude());
  grid[cellIndex.first][cellIndex.second].points.push_back(point);
}

// 删除点
void GridIndex::remove(const Point &point) {
  auto cellIndex = getCellIndex(point.getLongitude(), point.getLatitude());
  auto &points = grid[cellIndex.first][cellIndex.second].points;
  points.erase(std::remove(points.begin(), points.end(), point), points.end());
}

// 查找点
bool GridIndex::find(const Point &point) const {
  auto cellIndex = getCellIndex(point.getLongitude(), point.getLatitude());
  const auto &points = grid.at(cellIndex.first).at(cellIndex.second).points;
  return std::find(points.begin(), points.end(), point) != points.end();
}

// 范围查询
std::vector<Point> GridIndex::rangeQuery(double minLon, double maxLon,
                                         double minLat, double maxLat) const {
  std::vector<Point> result;
  int minX = getCellIndex(minLon, minLat).first;
  int maxX = getCellIndex(maxLon, maxLat).first;
  int minY = getCellIndex(minLon, minLat).second;
  int maxY = getCellIndex(maxLon, maxLat).second;

  for (int x = minX; x <= maxX; ++x) {
    for (int y = minY; y <= maxY; ++y) {
      if (grid.find(x) != grid.end() &&
          grid.at(x).find(y) != grid.at(x).end()) {
        const auto &points = grid.at(x).at(y).points;
        for (const auto &point : points) {
          if (point.getLongitude() >= minLon &&
              point.getLongitude() <= maxLon && point.getLatitude() >= minLat &&
              point.getLatitude() <= maxLat) {
            result.push_back(point);
          }
        }
      }
    }
  }

  return result;
}

// 计算网格单元索引
std::pair<int, int> GridIndex::getCellIndex(double longitude,
                                            double latitude) const {
  int xIndex = static_cast<int>((longitude - minLon) / cellSize);
  int yIndex = static_cast<int>((latitude - minLat) / cellSize);
  return {xIndex, yIndex};
}
