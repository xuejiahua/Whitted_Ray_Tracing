#include "../include/Color.h"

Color::Color(double r, double g, double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::Color(const Color& c) {
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
}

Color::~Color() {
}

double Color::get_r() const {
	return this->r;
}

double Color::get_g() const {
	return this->g;
}

double Color::get_b() const {
	return this->b;
}

void Color::set_r(const double r) {
	this->r = r;
}

void Color::set_g(const double g) {
	this->g = g;
}

void Color::set_b(const double b) {
	this->b = b;
}

void Color::set_rgb(const double r, const double g, const double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

double Color::operator[](int i) const {
	switch (i) {
		case 0:
			return this->r;
		case 1:
			return this->g;
		case 2:
			return this->b;
	}

	return 0.0;
}

Color& Color::operator=(const Color& c) {
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;

	return *this;
}

Color& Color::operator+=(const Color& c) {
	this->r += c.r;
	this->g += c.g;
	this->b += c.b;

	return *this;
}

Color& Color::operator*=(const Color& c) {
	this->r *= c.r;
	this->g *= c.g;
	this->b *= c.b;

	return *this;
}

Color operator+(const Color& c1, const Color& c2) {
	Color c = Color(c1.r + c2.r,
					c1.g + c2.g,
					c1.b + c2.b);

	return c;
}

Color operator*(const Color& c1, const Color& c2) {
	return Color(c1.r * c2.r,
				 c1.g * c2.g,
				 c1.b * c2.b);
}

Color operator*(const Color& c, const double k) {
	return Color(c.r * k,
				 c.g * k,
				 c.b * k);
}

Color operator*(const double k, const Color& c) {
	return Color(k * c.r,
				 k * c.g,
				 k * c.b);
}

Color operator/(const Color& c, const double k) {
	return Color(c.r / k,
				 c.g / k,
				 c.b / k);
}

void Color::clamp() {
	this->r = this->r > 1 ? 1 : (this->r < 0 ? 0 : this->r);
	this->g = this->g > 1 ? 1 : (this->g < 0 ? 0 : this->g);
	this->b = this->b > 1 ? 1 : (this->b < 0 ? 0 : this->b);
}

Color mix(const Color& c1, const double weight1,
		  const Color& c2, const double weight2) {
	return (c1 * weight1 + c2 * weight2) / (weight1 + weight2);
}