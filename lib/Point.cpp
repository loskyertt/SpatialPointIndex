#include "Point.h"

// 构造函数
Point::Point(double longitude, double latitude)
    : longitude(longitude), latitude(latitude) {}

// 获取经度
double Point::getLongitude() const {
  return longitude;
}

// 获取纬度
double Point::getLatitude() const {
  return latitude;
}

// 运算符重载，用于比较两个点是否相等
bool Point::operator==(const Point &other) const {
  return (longitude == other.longitude) && (latitude == other.latitude);
}

bool Point::operator!=(const Point &other) const {
  return !(*this == other);
}
