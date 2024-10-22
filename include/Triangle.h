#pragma once

#include <cmath>

#include "Primitive.h"

class Triangle: public Primitive {
public:
	// Constructors and Destructor
	// ---------------------------
	Triangle(const Point3   v1, const Point3   v2,  const Point3  v3,
			 const Point3  vt1, const Point3  vt2, const Point3  vt3,
			 const Vector3 vn1, const Vector3 vn2, const Vector3 vn3,
			 std::shared_ptr<Material> material,
			 std::shared_ptr<Texture> texture);
	Triangle(const Point3   v1, const Point3   v2, const Point3   v3,
			 const Point3  vt1, const Point3  vt2, const Point3  vt3,
			 std::shared_ptr<Material> material,
			 std::shared_ptr<Texture> texture);
	Triangle(const Point3   v1, const Point3   v2, const Point3   v3,
			 std::shared_ptr<Material> material,
			 std::shared_ptr<Texture> texture);
	~Triangle();

	// Overrided functions
	// -------------------
	virtual Point3 get_reference_point() const override;
	virtual bool hit(const Ray& ray, Hit_Record& record) const override;

	// Other functions
	// ---------------

private:
	virtual void compute_AABB() override;

private:
	Vector3                   normal;
	Point3                    vertices[3];
	Point3                    texture_vertices[3];

	std::shared_ptr<Material> material;
	std::shared_ptr<Texture>  texture;
};