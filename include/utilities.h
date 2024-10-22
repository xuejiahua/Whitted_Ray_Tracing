#pragma once

#include <iomanip>

#include "Primitive.h"
#include "Vector3.h"

// UTILITY FUNCTIONS
inline double clamp(const double x, const double min, const double max) {
	return x < min ? min : (x > max ? max : x);
}

inline double clamp(const double x) {
	return clamp(x, 0.0, 1.0);
}

inline void state_output(const int state, const double num, int spp = 0) {
	switch (state) {
		case 1:
			std::cout << "===== Framework Building ============================================" << std::endl;
			break;
		case 2:
			std::cout << "===== Framework Built ===============================================" << std::endl;
			std::cout << "\rTotal time used for framework building: "
				<< num / CLOCKS_PER_SEC << "s." << std::endl;
			break;
		case 3:
			std::cout << "===== Rendering =====================================================" << std::endl;
			break;
		case 4:
			std::cout << "spp:" << std::right << std::setw(4) << int(num + 0.1) + 1
				<< "/" << spp << ", ";
			break;
		case 5:
			std::cout.width(3);
			std::cout << int(num * 100) << "%";
			std::cout << "\b\b\b\b";
			break;
		case 6:
			std::cout << "time used : " << num / CLOCKS_PER_SEC << "s." << std::endl;
			break;
		case 7:
			std::cout << "===== Rendered ======================================================" << std::endl;
			std::cout << "\rTotal time used for rendering: "
				<< num / CLOCKS_PER_SEC << "s." << std::endl;
			break;
		case 8:
			std::cout << "===== Image Generated ===============================================" << std::endl;
	}
}

inline bool compare_x(const std::shared_ptr<Primitive> primitive1,
			   const std::shared_ptr<Primitive> primitive2) {
	return primitive1->get_reference_point().get_x() < primitive2->get_reference_point().get_x();
}

inline bool compare_y(const std::shared_ptr<Primitive> primitive1,
			   const std::shared_ptr<Primitive> primitive2) {
	return primitive1->get_reference_point().get_y() < primitive2->get_reference_point().get_y();
}

inline bool compare_z(const std::shared_ptr<Primitive> primitive1,
			   const std::shared_ptr<Primitive> primitive2) {
	return primitive1->get_reference_point().get_z() < primitive2->get_reference_point().get_z();
}

// RANDOM FUNCTIONS
// ----------------

// Returns a random real in [0, 1).
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min, max).
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

// Returns a random integer in [min, max].
inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

inline Vector3 random_vector_in_disk(const double radius) {
	Vector3 v;
	do {
		v.set_xyz(random_double(-radius, radius),
				  random_double(-radius, radius),
				  0.0);
	} while (v.length() > radius);

	return v;
}

inline Vector3 random_vector_in_sphere(const double radius) {
	Vector3 v;
	do {
		v.set_xyz(random_double(-radius, radius),
				  random_double(-radius, radius),
				  random_double(-radius, radius));
	} while (v.length() > radius);

	return v;
}