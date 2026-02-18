#include <chrono>
#include <iostream>

#include "PointManager.h"

void displayPoints(const std::vector<Point> &points, int page, int pageSize,
                   const Point *highlightPoint = nullptr,
                   const std::string &highlightTag = "") {
  auto start = std::chrono::high_resolution_clock::now();

  int totalPoints = points.size();
  int totalPages = (totalPoints + pageSize - 1) / pageSize;
  if (page < 1 || page > totalPages) {
    std::cout << "无效的页数。" << std::endl;
    return;
  }

  int startIdx = (page - 1) * pageSize;
  int endIdx = std::min(startIdx + pageSize, totalPoints);

  std::cout << "第 " << page << " 页的点数据：" << std::endl;
  for (int i = startIdx; i < endIdx; ++i) {
    std::cout.precision(8);
    std::cout << "经度: " << points[i].getLongitude()
              << ", 纬度: " << points[i].getLatitude();
    if (highlightPoint && points[i] == *highlightPoint) {
      std::cout << " " << highlightTag;
    }
    std::cout << std::endl;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "打印数据花费时间: " << elapsed.count() << " 秒" << std::endl;
}

void displayAllPoints(const std::vector<Point> &points) {
  auto start = std::chrono::high_resolution_clock::now();

  std::cout << "所有点数据：" << std::endl;
  for (const auto &point : points) {
    std::cout.precision(8);
    std::cout << "经度: " << point.getLongitude()
              << ", 纬度: " << point.getLatitude() << std::endl;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "打印数据花费时间: " << elapsed.count() << " 秒" << std::endl;
}

int main() {
  double minLon = 0.0, maxLon = 180.0, minLat = 0.0, maxLat = 90.0;
  double cellSize = 1.0;
  int pageSize = 50;

  PointManager manager(minLon, maxLon, minLat, maxLat, cellSize);
  std::string filename;

  std::cout << "请输入点数据文件的路径：";
  std::cin >> filename;

  try {
    manager.loadFromFile(filename);
    const auto &points =
        manager.rangeQuery(minLon, maxLon, minLat, maxLat);  // 获取所有点

    // 分页显示
    int page = 1;
    while (true) {
      std::cout << "请输入要查看的页数 (1-"
                << (points.size() + pageSize - 1) / pageSize
                << ")，或输入0退出：";
      std::cin >> page;
      if (page == 0)
        break;
      displayPoints(points, page, pageSize);
    }

    // 以下为其他管理点数据的功能
    std::string command;
    while (true) {
      std::cout << "请输入命令 (insert, remove, find, range, save, exit)：";
      std::cin >> command;

      if (command == "insert") {
        double lon, lat;
        std::cout << "请输入点的经度和纬度：";
        std::cin >> lon >> lat;
        Point newPoint(lon, lat);
        manager.insertPoint(newPoint);
        std::cout << "点已插入。" << std::endl;
        const auto &updatedPoints =
            manager.rangeQuery(minLon, maxLon, minLat, maxLat);
        int insertedPage =
            ((std::find(updatedPoints.begin(), updatedPoints.end(), newPoint) -
              updatedPoints.begin()) /
             pageSize) +
            1;
        displayPoints(updatedPoints, insertedPage, pageSize, &newPoint, "new");
      } else if (command == "remove") {
        double lon, lat;
        std::cout << "请输入要删除点的经度和纬度：";
        std::cin >> lon >> lat;
        manager.removePoint(Point(lon, lat));
        std::cout << "点已删除。" << std::endl;
      } else if (command == "find") {
        double lon, lat;
        std::cout << "请输入要查找点的经度和纬度：";
        std::cin >> lon >> lat;
        Point findPoint(lon, lat);
        const auto &updatedPoints =
            manager.rangeQuery(minLon, maxLon, minLat, maxLat);
        bool found = manager.findPoint(findPoint);
        if (found) {
          int foundPage = ((std::find(updatedPoints.begin(),
                                      updatedPoints.end(), findPoint) -
                            updatedPoints.begin()) /
                           pageSize) +
                          1;
          displayPoints(updatedPoints, foundPage, pageSize, &findPoint, "*");
        } else {
          std::cout << "点不存在。" << std::endl;
        }
      } else if (command == "range") {
        double minLon, maxLon, minLat, maxLat;
        std::cout << "请输入查询范围的最小经度、最大经度、最小纬度和最大纬度：";
        std::cin >> minLon >> maxLon >> minLat >> maxLat;
        auto pointsInRange = manager.rangeQuery(minLon, maxLon, minLat, maxLat);
        displayAllPoints(pointsInRange);
      } else if (command == "save") {
        std::string saveFilename;
        std::cout << "请输入要保存的文件路径：";
        std::cin >> saveFilename;
        manager.saveToFile(saveFilename);
        std::cout << "点数据已保存到文件。" << std::endl;
      } else if (command == "exit") {
        break;
      } else {
        std::cout << "未知命令，请重试。" << std::endl;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "发生错误：" << e.what() << std::endl;
  }

  return 0;
}
