#pragma once

#include <cmath>

#include "Ray.h"
#include "Point3.h"
#include "Vector3.h"
#include "Material.h"
#include "Primitive.h"
#include "Hit_Record.h"
#include "constants.h"

class Sphere: public Primitive {
public:
	// Constructors and Destructor
	// ---------------------------
	Sphere(Point3 center, double radius, 
		   std::shared_ptr<Material> material,
		   std::shared_ptr<Texture> texture);
	Sphere(const Sphere& s);
	~Sphere();

	// Overrided operators
	// -------------------
	Sphere& operator=(const Sphere& s);

	// Other functions
	// ---------------
	virtual bool hit(const Ray& ray, Hit_Record& record) const override;
	virtual Point3 get_reference_point() const override;

private:
	virtual void compute_AABB() override;

private:
	Point3 center;
	double radius;
};