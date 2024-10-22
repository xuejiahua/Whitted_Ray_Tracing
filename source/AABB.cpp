#include "../include/AABB.h"

AABB::AABB(Point3 minimum, Point3 maximum) {
	this->minimum = minimum;
	this->maximum = maximum;
}

AABB::AABB(const AABB& aabb) {
	this->minimum = aabb.minimum;
	this->maximum = aabb.maximum;
}

AABB::~AABB() {
}

double AABB::get_x_span() const {
	return maximum.get_x() - minimum.get_x();
}

double AABB::get_y_span() const {
	return maximum.get_y() - minimum.get_y();
}

double AABB::get_z_span() const {
	return maximum.get_z() - minimum.get_z();
}

double AABB::get_surface() const {
	return 2.0 * (  this->get_x_span() * this->get_y_span()
				  + this->get_y_span() * this->get_z_span()
				  + this->get_z_span() * this->get_x_span());
}

void AABB::set_AABB(Point3 minimum, Point3 maximum) {
	this->minimum = minimum;
	this->maximum = maximum;
}

AABB& AABB::operator=(const AABB& aabb) {
	this->minimum = aabb.minimum;
	this->maximum = aabb.maximum;

	return *this;
}

void AABB::U(const AABB& aabb) {
	this->U(aabb.minimum, aabb.maximum);
}

void AABB::U(const Point3& minimum, const Point3& maximum) {
	this->minimum.set_xyz(fmin(this->minimum.get_x(), minimum.get_x()),
						  fmin(this->minimum.get_y(), minimum.get_y()),
						  fmin(this->minimum.get_z(), minimum.get_z()));
	this->maximum.set_xyz(fmax(this->maximum.get_x(), maximum.get_x()),
						  fmax(this->maximum.get_y(), maximum.get_y()),
						  fmax(this->maximum.get_z(), maximum.get_z()));
}

bool AABB::hit(const Ray& ray) {
	double t_min = TOLERANCE, t_max = INF;

	for (int axis = 0; axis <= 2; axis++) {
		double invD = 1.0f / ray.get_direction()[axis];
		double t_in  = (this->minimum[axis] - ray.get_origin()[axis]) * invD;
		double t_out = (this->maximum[axis] - ray.get_origin()[axis]) * invD;

		if (invD < 0.0f) {
			std::swap(t_in, t_out);
		}

		t_min = t_in  > t_min ? t_in  : t_min;
		t_max = t_out < t_max ? t_out : t_max;

		if (t_max < t_min) {
			return false;
		}
	}

	return true;
}

void AABB::avoid_plane() {
	if (this->get_x_span() < TOLERANCE && this->get_x_span() > -TOLERANCE) {
		this->minimum.set_x(this->minimum.get_x() - TOLERANCE);
		this->maximum.set_x(this->maximum.get_x() + TOLERANCE);
	}
	if (this->get_y_span() < TOLERANCE && this->get_y_span() > -TOLERANCE) {
		this->minimum.set_y(this->minimum.get_y() - TOLERANCE);
		this->maximum.set_y(this->maximum.get_y() + TOLERANCE);
	}
	if (this->get_z_span() < TOLERANCE && this->get_z_span() > -TOLERANCE) {
		this->minimum.set_z(this->minimum.get_z() - TOLERANCE);
		this->maximum.set_z(this->maximum.get_z() + TOLERANCE);
	}
}

AABB U(const AABB& aabb1, const AABB& aabb2) {
	return AABB(Point3(fmin(aabb1.minimum.get_x(), aabb2.minimum.get_x()),
					   fmin(aabb1.minimum.get_y(), aabb2.minimum.get_y()),
					   fmin(aabb1.minimum.get_z(), aabb2.minimum.get_z())),
				Point3(fmax(aabb1.maximum.get_x(), aabb2.maximum.get_x()),
					   fmax(aabb1.maximum.get_y(), aabb2.maximum.get_y()),
					   fmax(aabb1.maximum.get_z(), aabb2.maximum.get_z())));
}