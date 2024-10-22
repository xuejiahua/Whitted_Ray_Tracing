#pragma once

#include "Vector3.h"

class Point3 {
public:
	// Constructors and Destructors.
	// -----------------------------
	Point3(double x = 0.0,
		   double y = 0.0,
		   double z = 0.0);
	Point3(const Point3& p);
	~Point3();

	// Getters and Setters.
	// --------------------
	double get_x() const;
	double get_y() const;
	double get_z() const;

	void set_x(double x);
	void set_y(double y);
	void set_z(double z);
	void set_xyz(double x, double y, double z);

	// Overloaded operators.
	// ---------------------
	Point3&        operator=(const Point3& p);
	double         operator[](int i) const;

	Point3&        operator+=(const Vector3& v);
	Point3&        operator-=(const Vector3& v);

	friend Point3  operator+(const Point3& p, const Vector3& v);
	friend Point3  operator-(const Point3& p, const Vector3& v);
	friend Vector3 operator-(const Point3& p1, const Point3& p2);

private:
	double x, y, z;
};