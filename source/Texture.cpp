#include "../include/Texture.h"

#include "../external/stb_image.h"
#include "../include/utilities.h"


Texture::Texture(int texture_type, Color color) {
	this->texture_type = texture_type;
	this->color1       = color;

	sticker_data       = NULL;
}

Texture::Texture(int texture_type, double checker_density, Color color1, Color color2) {
	this->texture_type = texture_type;
	this->checker_density = checker_density;
	this->color1 = color1;
	this->color2 = color2;

	sticker_data = NULL;
}

Texture::Texture(int texture_type, const char* texture_sticker_filepath) {
	this->texture_type = texture_type;
	auto components_per_pixel = BYTES_PER_PIXEL;

	sticker_data = stbi_load(texture_sticker_filepath, 
							 &sticker_width, &sticker_height, 
							 &components_per_pixel, components_per_pixel);

	if (!sticker_data) {
		std::cerr << "ERROR: Could not load texture image file '" << texture_sticker_filepath << "'.\n";
		sticker_width = sticker_height = 0;
	}
}

Texture::~Texture() {
	if (sticker_data) {
		delete sticker_data;
	}
}

Color Texture::albedo(Point3 texture_point) const {
	switch (this->texture_type) {
		case 1:	{
			return this->color1;
			break;
		}

		case 2:	{
			return (sin(this->checker_density * 2 * PI * texture_point.get_x())
			      * sin(this->checker_density * 2 * PI * texture_point.get_y())) < 0.0 ? this->color1 : this->color2;
		}

		case 3:	{
			int texture_u = fmin(static_cast<int>(clamp(      texture_point.get_x(), 0.0, 1.0) * sticker_width),  sticker_width  - 1);
			int texture_v = fmin(static_cast<int>(clamp(1.0 - texture_point.get_y(), 0.0, 1.0) * sticker_height), sticker_height - 1);

			const double color_scale = 1.0 / 255.999;
			unsigned char* pixel = sticker_data + texture_v * BYTES_PER_PIXEL * sticker_width + texture_u * BYTES_PER_PIXEL;

			return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
		}
	}

	return Color(1.0, 0.0, 0.0);
}