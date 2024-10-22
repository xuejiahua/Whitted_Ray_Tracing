#pragma once

#include "Color.h"
#include "Point3.h"
#include "Vector3.h"
#include "Material.h"
#include "Texture.h"

class Ray;

struct Hit_Record {
	double                    distance;
	Point3                    point;
	Point3                    texture_point;
	Vector3                   normal;                                     // unit normal vector
	bool                      front_face;
	Color                     light_pass_proportion;

	std::shared_ptr<Material> material;
	std::shared_ptr<Texture>  texture;
};