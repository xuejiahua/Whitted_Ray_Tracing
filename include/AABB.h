#pragma once

#include "constants.h"
#include "Point3.h"
#include "Ray.h"

class AABB {
public:
	// Constructors and Destructor
	// ---------------------------
	AABB(Point3 minimum = Point3( INF,  INF,  INF),
		 Point3 maximum = Point3(-INF, -INF, -INF));
	AABB(const AABB& aabb);
	~AABB();

	// Getters and Setters
	// -------------------
	double get_x_span() const;
	double get_y_span() const;
	double get_z_span() const;
	double get_surface() const;
	void set_AABB(Point3 minimum, Point3 maximum);

	// Overloaded operators
	// --------------------
	AABB& operator=(const AABB& aabb);

	// Other functions
	// ---------------
	void U(const Point3& minimum, const Point3& maximum);
	void U(const AABB& aabb);
	bool hit(const Ray& ray);
	friend AABB U(const AABB& aabb1, const AABB& aabb2);
	void avoid_plane();

private:
	Point3 minimum;
	Point3 maximum;
};