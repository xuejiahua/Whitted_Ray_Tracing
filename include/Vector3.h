#pragma once

#include <cmath>
#include <iostream>

class Vector3 {
public:
	// Constructors and Destructor.
	// ----------------------------
	Vector3(double x = 0.0,
			double y = 0.0,
			double z = 0.0);
	Vector3(const Vector3& v);
	~Vector3();

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
	// ----------------------
	Vector3        operator-() const;
	Vector3&       operator=(const Vector3 v);
	double         operator[](int i) const;

	Vector3&       operator+=(const Vector3& v);
	Vector3&       operator-=(const Vector3& v);
	Vector3&       operator*=(const double k);
	Vector3&       operator/=(const double k);

	friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
	friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
	friend Vector3 operator*(const double k, const Vector3& v);
	friend Vector3 operator*(const Vector3& v, const double k);
	friend Vector3 operator/(const Vector3& v, const double k);

	// Other functions.
	// ----------------
	double  length() const;
	double  length_square() const;
	void    unitization();
	Vector3 get_unit_vector();
	bool    near_zero() const;

	friend double  dot_product(const Vector3& v1, const Vector3& v2);
	friend Vector3 cross_product(const Vector3& v1, const Vector3& v2);

private:
	double x, y, z;
};