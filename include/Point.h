#ifndef POINT_H
#define POINT_H

class Point {
 private:
  double longitude;  // 经度
  double latitude;   // 纬度

 public:
  // 构造函数
  Point(double longitude, double latitude);

  // 获取经度
  double getLongitude() const;

  // 获取纬度
  double getLatitude() const;

  // 运算符重载，用于比较两个点是否相等
  bool operator==(const Point &other) const;
  bool operator!=(const Point &other) const;
};

#endif  // POINT_H
