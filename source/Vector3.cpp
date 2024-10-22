#include "../include/Vector3.h"

#include "../include/constants.h"

Vector3::Vector3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3& v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

Vector3::~Vector3() {
}

double Vector3::get_x() const {
	return this->x;
}

double Vector3::get_y() const {
	return this->y;
}

double Vector3::get_z() const {
	return this->z;
}

void Vector3::set_x(double x) {
	this->x = x;
}

void Vector3::set_y(double y) {
	this->y = y;
}

void Vector3::set_z(double z) {
	this->z = z;
}

void Vector3::set_xyz(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator-() const {
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3& Vector3::operator=(const Vector3 v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;

	return *this;
}

double Vector3::operator[](int i) const {
	switch (i) {
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
	}

	return 0.0;
}

Vector3& Vector3::operator+=(const Vector3& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}

Vector3& Vector3::operator*=(const double k) {
	this->x *= k;
	this->y *= k;
	this->z *= k;

	return *this;
}

Vector3& Vector3::operator/=(const double k) {
	this->x /= k;
	this->y /= k;
	this->z /= k;

	return *this;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x,
				   v1.y + v2.y,
				   v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x,
				   v1.y - v2.y,
				   v1.z - v2.z);
}

Vector3 operator*(const double k, const Vector3& v) {
	return Vector3(k * v.x,
				   k * v.y,
				   k * v.z);
}

Vector3 operator*(const Vector3& v, const double k) {
	return Vector3(v.x * k,
				   v.y * k,
				   v.z * k);
}

Vector3 operator/(const Vector3& v, const double k) {
	return Vector3(v.x / k,
				   v.y / k,
				   v.z / k);
}

double Vector3::length() const {
	return sqrt(this->length_square());
}

double Vector3::length_square() const {
	return this->x * this->x 
		 + this->y * this->y 
		 + this->z * this->z;
}

void Vector3::unitization() {
	*this /= this->length();
}

Vector3 Vector3::get_unit_vector() {
	return Vector3(*this / this->length());
}

// Returns true if the vector is close to zero-vector.
bool Vector3::near_zero() const {
	return (fabs(this->x) < TOLERANCE)
		&& (fabs(this->y) < TOLERANCE)
		&& (fabs(this->z) < TOLERANCE);
}

double dot_product(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x
		 + v1.y * v2.y
		 + v1.z * v2.z;
}

Vector3 cross_product(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y,
				   v1.z * v2.x - v1.x * v2.z,
				   v1.x * v2.y - v1.y * v2.x);
}