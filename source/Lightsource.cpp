#include "../include/Lightsource.h"

#include "../include/constants.h"

Lightsource::Lightsource(Color color, Point3 position, double intensity) {
	this->color     = color;
	this->position  = position;
	this->intensity = intensity;
}

Lightsource::Lightsource(const Lightsource& l) {
	this->color     = l.color;
	this->position  = l.position;
	this->intensity = l.intensity;
}

Lightsource::~Lightsource(void) {
}

Color Lightsource::get_color() const {
	return this->color;
}

Point3 Lightsource::get_position() const {
	return this->position;
}

double Lightsource::get_intensity(double distance) const {
	return this->intensity / (4 * PI * pow(distance, 2));
}
