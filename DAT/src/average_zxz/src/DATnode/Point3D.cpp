#include "Point3D.h"


Point3D::Point3D()
{
}


Point3D::~Point3D()
{

}



Point3D Point3D::operator-(const Point3D &b)
{
  return Point3D(this->x - b.x, this->y - b.y, this->z - b.z);
}
Point3D Point3D::operator+(const Point3D &b)
{
  return Point3D(this->x + b.x, this->y + b.y, this->z + b.z);
}
double Point3D::operator*(const Point3D &b)
{
  return this->x * b.x + this->y * b.y + this->z * b.z;
}
Point3D Point3D::operator*(const double &b)
{
  return Point3D(this->x * b, this->y * b, this->z * b);
}
Point3D Point3D::operator/(const double &b)
{
  return Point3D(this->x / b, this->y / b, this->z / b);
}

Point3D::Point3D(double x1, double y1,double z1)
{
	this->x = x1;
	this->y = y1;
  this->z = z1;
}

double Point3D::norm1()
{
  return std::sqrt(this->norm2());
}
double Point3D::norm2()
{
  return this->x*this->x+this->y*this->y+this->z*this->z;
}

Point3D Point3D::unit()
{
    double d=this->norm1();
    if(d<0.001)return Point3D(0,0,0);
    else return Point3D(this->x/d,this->y/d,this->z/d);
}
