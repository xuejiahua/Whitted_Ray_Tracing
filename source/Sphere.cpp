#include "../include/Sphere.h"

Sphere::Sphere(Point3 center, double radius, 
			   std::shared_ptr<Material> material,
			   std::shared_ptr<Texture> texture) {
	this->center   = center;
	this->radius   = radius;
	this->material = material;
	this->texture  = texture;
	this->compute_AABB();
}

Sphere::Sphere(const Sphere& s) {
	this->center   = s.center;
	this->radius   = s.radius;
	this->material = s.material;
	this->texture  = s.texture;
	this->aabb     = s.aabb;
}

Sphere::~Sphere() {
}

Sphere& Sphere::operator=(const Sphere& s) {
	this->center   = s.center;
	this->radius   = s.radius;
	this->material = s.material;
	this->texture  = s.texture;
	this->aabb     = s.aabb;

	return *this;
}

void Sphere::compute_AABB() {
	this->aabb.set_AABB(Point3(center.get_x() - fabs(radius),
							   center.get_y() - fabs(radius),
							   center.get_z() - fabs(radius)),
						Point3(center.get_x() + fabs(radius),
							   center.get_y() + fabs(radius),
							   center.get_z() + fabs(radius)));
	this->aabb.avoid_plane();
}

bool Sphere::hit(const Ray& ray, Hit_Record& record) const {
	Vector3 oc = ray.get_origin() - center;
	auto a = pow(ray.get_direction().length(), 2);
	auto half_b = dot_product(oc, ray.get_direction());
	auto c = dot_product(oc, oc) - pow(this->radius,2);
	auto discriminant = pow(half_b, 2) - a * c;
	if (discriminant < TOLERANCE) {
		return false;
	}

	// Find the nearest root in the acceptable range [DEFAULT_TOLERANCE, record.distance].
	auto root = (-half_b - sqrt(discriminant)) / a;
	if (root < TOLERANCE || root > record.distance) {
		root = (-half_b + sqrt(discriminant)) / a;
		if (root < TOLERANCE || root > record.distance) {
			return false;
		}
	}

	Vector3 outward_normal = (ray.at(root) - this->center) / radius;
	double theta = acos(-outward_normal.get_y());
	double phi = -atan2(-outward_normal.get_z(), outward_normal.get_x()) + PI;

	record.distance      = root;
	record.point         = ray.at(root);
	record.texture_point = Point3(1 - phi / (2 * PI), theta / PI, 0.0);
	record.front_face    = dot_product(ray.get_direction(), outward_normal) < 0;
	record.normal        = record.front_face ? outward_normal :
		                                      -outward_normal;
	record.material      = this->material;
	record.texture       = this->texture;
	record.light_pass_proportion *= 0.5 * this->material->get_refraction_coefficient() * record.texture->albedo(record.texture_point);

	return true;
}

Point3 Sphere::get_reference_point() const {
	return this->center;
}
