#include "../include/Ray.h"

#include "../include/utilities.h"

Ray::Ray(Point3 origin, Vector3 direction) {
	this->origin    = origin;
	this->direction = direction.get_unit_vector();
}

Ray::Ray(const Ray& r) {
	this->origin    = r.origin;
	this->direction = r.direction;
}

Ray::~Ray() {
}

Point3 Ray::get_origin() const {
	return origin;
}

Vector3 Ray::get_direction() const {
	return direction;
}

void Ray::set_origin(Point3& origin) {
	this->origin = origin;
}

void Ray::set_direction(Vector3& direction) {
	this->direction = direction.get_unit_vector();
}

Ray& Ray::operator=(const Ray& r) {
	this->origin = r.origin;
	this->direction = r.direction;

	return *this;
}

Point3 Ray::at(const double t) const {
	return this->origin + t * this->direction;
}

Ray Ray::specular_reflect(const Hit_Record& record) const {
	if (record.material->get_specular_fuzz() > TOLERANCE) {
		return Ray(record.point,
				   this->direction - 2 * dot_product(this->direction, record.normal) * record.normal
				   + random_vector_in_sphere(record.material->get_specular_fuzz()));
	} else {
		return Ray(record.point,
				   this->direction - 2 * dot_product(this->direction, record.normal) * record.normal);
	}
}

Ray Ray::refract(const Hit_Record& record) const {
	const double ratio_of_indices = record.front_face ? 1.0 / record.material->get_refractive_index() :
				                                        record.material->get_refractive_index();
	auto cos_theta = fabs(clamp(dot_product(-this->direction, record.normal), -1.0, 1.0));
	double k = 1 - pow(ratio_of_indices, 2) * (1 - pow(cos_theta, 2));
	return Ray(record.point, 
			   ratio_of_indices * this->direction
		       + (ratio_of_indices * cos_theta - sqrt(k)) * record.normal);
}