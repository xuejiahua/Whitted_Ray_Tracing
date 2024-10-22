#pragma once

#include <string>
#include <iostream>

#include "Color.h"
#include "Config.h"
#include "constants.h"

#pragma pack(2)
typedef struct {
	unsigned short  fh_type;
	unsigned long   fh_file_size;
	unsigned short  fh_reserved1;
	unsigned short  fh_reserved2;
	unsigned long   fh_offset;
} Bitmap_File_Header;
#pragma pack()

#pragma pack(2)
typedef struct {
	unsigned long   ih_header_size;
	long            ih_image_width;
	long            ih_image_height;
	unsigned short  ih_planes;
	unsigned short  ih_bits_per_pixel;
	unsigned long   ih_compression;
	unsigned long   ih_image_size;
	long            ih_X_pixels_per_meter;
	long            ih_Y_pixels_per_meter;
	unsigned long   ih_color_used;
	unsigned long   ih_color_important;
} Bitmap_Infomation_Header;
#pragma pack()

class Canvas {
public:
	// Constructor and Destructor
	// --------------------------
	Canvas(const std::shared_ptr<Config> config);
	~Canvas();

	// Other functions
	// ---------------
	Color get_pixel_color(const int row, const int column);
	void  add_pixel_color(const int row, const int column, const Color& color);
	void  set_pixel_color(const int row, const int column, const Color& color);

	void generate_image(const bool   is_final_image,
						const int    samples_count,
						const double seconds);

private:
	void create_output_folders();
	void generate_image(const char* filename, int samples_count);

private:
	std::shared_ptr<Config>  config;

	int                      width, height;
	int                      output_struction_option;
	double                   gamma_correction_coefficient;
	Color*                   pixel_color;
	unsigned char*           output_RGB;
	std::string              final_image_prefix;
	std::string              samples_image_prefix;
};