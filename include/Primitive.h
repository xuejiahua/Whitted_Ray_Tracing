#pragma once

#include "AABB.h"
#include "constants.h"
#include "Hit_Record.h"
#include "Point3.h"
#include "Ray.h"

class Primitive {
public:
	// Constructors and Destructor
	// ---------------------------
	virtual AABB& get_AABB_reference();
	virtual AABB  get_AABB();

	// Other functions
	// ---------------
	virtual void compute_AABB() = 0;
	virtual bool hit(const Ray& ray, Hit_Record& record) const = 0;
	virtual Point3 get_reference_point() const = 0;

protected:
	AABB                      aabb     = AABB(Point3( INF,  INF,  INF),
					                          Point3(-INF, -INF, -INF));

	std::shared_ptr<Material> material;
	std::shared_ptr<Texture>  texture;
};