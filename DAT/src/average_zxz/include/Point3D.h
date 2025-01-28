#ifndef POINT3D_H_
#define POINT3D_H_
#include <cmath>
class Point3D
{
public:
  Point3D();
  ~Point3D();

  Point3D(double x1, double y1,double z1);
  Point3D operator-(const Point3D &b);
  Point3D operator+(const Point3D &b);
  double operator*(const Point3D &b);
  Point3D operator/(const double &b);
  Point3D operator*(const double &b);
	double x;
	double y;
  double z;

  double norm1();
  double norm2();
  Point3D unit();

};


#endif

