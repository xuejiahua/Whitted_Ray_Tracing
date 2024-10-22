#include "../include/Point3.h"

Point3::Point3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point3::Point3(const Point3& p) {
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
}

Point3::~Point3() {
}

double Point3::get_x() const {
	return this->x;
}

double Point3::get_y() const {
    return this->y;
}

double Point3::get_z() const {
    return this->z;
}

void Point3::set_x(double x) {
    this->x = x;
}

void Point3::set_y(double y) {
    this->y = y;
}

void Point3::set_z(double z) {
    this->z = z;
}

void Point3::set_xyz(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3& Point3::operator=(const Point3& p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;

    return *this;
}

double Point3::operator[](int i) const {
    switch (i) {
        case 0:
            return this->x;
        case 1:
            return this->y;
        case 2:
            return this->z;
    }

    return 0;
}

Point3& Point3::operator+=(const Vector3& v) {
    this->x += v.get_x();
    this->y += v.get_y();
    this->z += v.get_z();

    return *this;
}

Point3& Point3::operator-=(const Vector3& v) {
    this->x -= v.get_x();
    this->y -= v.get_y();
    this->z -= v.get_z();

    return *this;
}

Point3 operator+(const Point3& p, const Vector3& v) {
    return Point3(p.x + v.get_x(),
                  p.y + v.get_y(),
                  p.z + v.get_z());
}

Point3 operator-(const Point3& p, const Vector3& v) {
    return Point3(p.x - v.get_x(),
                  p.y - v.get_y(),
                  p.z - v.get_z());
}

Vector3 operator-(const Point3& p1, const Point3& p2) {
    return Vector3(p1.x - p2.x,
                   p1.y - p2.y,
                   p1.z - p2.z);
}