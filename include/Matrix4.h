#pragma once

#include "Point3.h"
#include "Vector3.h"

class Matrix4 {
public:
	// Constructors and Destructor
	// ---------------------------
	Matrix4(double matrix_11 = 0.0, double matrix_12 = 0.0, double matrix_13 = 0.0, double matrix_14 = 0.0,
			double matrix_21 = 0.0, double matrix_22 = 0.0, double matrix_23 = 0.0, double matrix_24 = 0.0, 
			double matrix_31 = 0.0, double matrix_32 = 0.0, double matrix_33 = 0.0, double matrix_34 = 0.0, 
			double matrix_41 = 0.0, double matrix_42 = 0.0, double matrix_43 = 0.0, double matrix_44 = 0.0);
	Matrix4(const Matrix4& m);
	~Matrix4();

	// Getters and Setters
	// -------------------
	void set_item(int row, int column, double value);
	void set_matrix(double matrix_11 = 0.0, double matrix_12 = 0.0, double matrix_13 = 0.0, double matrix_14 = 0.0,
					double matrix_21 = 0.0, double matrix_22 = 0.0, double matrix_23 = 0.0, double matrix_24 = 0.0,
					double matrix_31 = 0.0, double matrix_32 = 0.0, double matrix_33 = 0.0, double matrix_34 = 0.0,
					double matrix_41 = 0.0, double matrix_42 = 0.0, double matrix_43 = 0.0, double matrix_44 = 0.0);

	// Overloaded operators
	// --------------------
	Matrix4& operator=(const Matrix4& m);

	friend Matrix4 operator*(const Matrix4 m1, const Matrix4 m2);

	// Other functions
	// ---------------
	void indentify();
	Point3  affine_transform(const Point3  p);
	Vector3 affine_transform(const Vector3 v);

private:
	double matrix[4][4];
};