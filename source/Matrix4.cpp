#include "../include/Matrix4.h"

Matrix4::Matrix4(double matrix_11, double matrix_12, double matrix_13, double matrix_14, 
				 double matrix_21, double matrix_22, double matrix_23, double matrix_24, 
				 double matrix_31, double matrix_32, double matrix_33, double matrix_34, 
				 double matrix_41, double matrix_42, double matrix_43, double matrix_44) {
	this->matrix[0][0] = matrix_11;
	this->matrix[0][1] = matrix_12;
	this->matrix[0][2] = matrix_13;
	this->matrix[0][3] = matrix_14;

	this->matrix[1][0] = matrix_21;
	this->matrix[1][1] = matrix_22;
	this->matrix[1][2] = matrix_23;
	this->matrix[1][3] = matrix_24;

	this->matrix[2][0] = matrix_31;
	this->matrix[2][1] = matrix_32;
	this->matrix[2][2] = matrix_33;
	this->matrix[2][3] = matrix_34;

	this->matrix[3][0] = matrix_41;
	this->matrix[3][1] = matrix_42;
	this->matrix[3][2] = matrix_43;
	this->matrix[3][3] = matrix_44;
}

Matrix4::Matrix4(const Matrix4& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = m.matrix[i][j];
		}
	}
}

Matrix4::~Matrix4() {
}

void Matrix4::set_item(int row, int column, double value) {
	this->matrix[row][column] = value;
}

void Matrix4::set_matrix(double matrix_11, double matrix_12, double matrix_13, double matrix_14,
						 double matrix_21, double matrix_22, double matrix_23, double matrix_24, 
						 double matrix_31, double matrix_32, double matrix_33, double matrix_34, 
						 double matrix_41, double matrix_42, double matrix_43, double matrix_44) {
	this->matrix[0][0] = matrix_11;
	this->matrix[0][1] = matrix_12;
	this->matrix[0][2] = matrix_13;
	this->matrix[0][3] = matrix_14;

	this->matrix[1][0] = matrix_21;
	this->matrix[1][1] = matrix_22;
	this->matrix[1][2] = matrix_23;
	this->matrix[1][3] = matrix_24;

	this->matrix[2][0] = matrix_31;
	this->matrix[2][1] = matrix_32;
	this->matrix[2][2] = matrix_33;
	this->matrix[2][3] = matrix_34;

	this->matrix[3][0] = matrix_41;
	this->matrix[3][1] = matrix_42;
	this->matrix[3][2] = matrix_43;
	this->matrix[3][3] = matrix_44;
}

Matrix4& Matrix4::operator=(const Matrix4& m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = m.matrix[i][j];
		}
	}

	return *this;
}

Matrix4 operator*(const Matrix4 m1, const Matrix4 m2) {
	Matrix4 m;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.matrix[i][j] = m1.matrix[i][0] * m2.matrix[0][j]
				           + m1.matrix[i][1] * m2.matrix[1][j]
				           + m1.matrix[i][2] * m2.matrix[2][j]
				           + m1.matrix[i][3] * m2.matrix[3][j];
		}
	}

	return m;
}

void Matrix4::indentify() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = i != j ? 0.0 : 1.0;
		}
	}
}

Point3 Matrix4::affine_transform(const Point3 p) {
	return Point3(this->matrix[0][0] * p.get_x() + this->matrix[0][1] * p.get_y() + this->matrix[0][2] * p.get_z() + this->matrix[0][3],
				  this->matrix[1][0] * p.get_x() + this->matrix[1][1] * p.get_y() + this->matrix[1][2] * p.get_z() + this->matrix[1][3],
				  this->matrix[2][0] * p.get_x() + this->matrix[2][1] * p.get_y() + this->matrix[2][2] * p.get_z() + this->matrix[2][3]);
}

Vector3 Matrix4::affine_transform(const Vector3 v) {
	return Vector3(this->matrix[0][0] * v.get_x() + this->matrix[0][1] * v.get_y() + this->matrix[0][2] * v.get_z(),
				   this->matrix[1][0] * v.get_x() + this->matrix[1][1] * v.get_y() + this->matrix[1][2] * v.get_z(),
				   this->matrix[2][0] * v.get_x() + this->matrix[2][1] * v.get_y() + this->matrix[2][2] * v.get_z());
}
