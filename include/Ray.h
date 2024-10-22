#pragma once

#include "constants.h"
#include "Hit_Record.h"
#include "Point3.h"
#include "Vector3.h"

class Ray {
public:
	// Constructors and Destructor.
	// ----------------------------
	Ray(Point3 origin     = Point3(0.0, 0.0, 0.0),
		Vector3 direction = Vector3(1.0, 0.0, 0.0));
	Ray(const Ray& r);
	~Ray();

	// Getters and Setters.
	// --------------------
	Point3  get_origin()    const;
	Vector3 get_direction() const;

	void set_origin(Point3& origin);
	void set_direction(Vector3& direction);

	// Reloaded operators.
	// -------------------
	Ray& operator=(const Ray& r);

	// Other functions.
	// ----------------
	Point3 at(const double t) const;
	Ray specular_reflect(const Hit_Record& record) const;
	Ray refract(const Hit_Record& record) const;

private:
	Point3  origin;
	Vector3 direction;
};