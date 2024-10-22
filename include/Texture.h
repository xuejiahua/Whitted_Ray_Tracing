#pragma once

#include "Color.h"
#include "constants.h"
#include "Point3.h"

class Texture {
public:
	// Constructors and Destructor
	// ---------------------------
	Texture(int texture_type                     = 1,
			Color color1                         = Color(1.0, 1.0, 1.0));
	Texture(int texture_type                     = 2,
			double checker_density               = 10.0,
			Color color1                         = Color(1.0, 1.0, 1.0),
			Color color2                         = Color(1.0, 1.0, 1.0));
	Texture(int texture_type                     = 3,
			const char* texture_sticker_filepath = NULL);
	~Texture();

	// Other functions
	// ---------------
	virtual Color albedo(Point3 texture_point) const;

private:
	int            texture_type;
	Color          color1, color2;
	double         checker_density;

	unsigned char* sticker_data;
	int            sticker_width, sticker_height;
	int            bytes_per_scanline;
};