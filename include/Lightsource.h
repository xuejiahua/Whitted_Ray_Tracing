#pragma once

#include "Color.h"
#include "Point3.h" 

class Lightsource {
public:
	// Constructors and Destructor
	// ---------------------------
	Lightsource(Color  color     = Color(1.0, 1.0, 1.0),
				Point3 position  = Point3(0.0, 0.0, 0.0),
				double intensity = 100.0);
	Lightsource(const Lightsource& l);
	virtual ~Lightsource(void);

	// Getters
	// -------
	Color  get_color() const;
	Point3 get_position() const;
	double get_intensity(const double distance) const;

private:
	Color  color;
	Point3 position;
	double intensity;
};
