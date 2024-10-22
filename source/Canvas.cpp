#include "../include/Canvas.h"

#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <iomanip>
#include <iostream>
#include <sstream>

Canvas::Canvas(const std::shared_ptr<Config> config) {
	this->config          = config;
	this->width           = config->get_IMAGE_WIDTH();
	this->height          = config->get_IMAGE_HEIGHT();
	this->output_struction_option = config->get_OUTPUT_STRUCTURE_OPTION();
	this->gamma_correction_coefficient = config->get_OUTPUT_GAMMA_CORRECTION_COEFFICIENT();
	this->final_image_prefix = config->get_OUTPUT_FOLDER_PATH() + "\\" + config->get_OUTPUT_PROJECT_NAME()
		+ (config->get_OUTPUT_STRUCTURE_OPTION() == -1 ? "" : "\\" + config->get_OUTPUT_PROJECT_NAME());
	this->samples_image_prefix = config->get_OUTPUT_FOLDER_PATH() + "\\" + config->get_OUTPUT_PROJECT_NAME() + "\\samples\\";

	this->pixel_color     = new Color[this->width * this->height];
	this->output_RGB      = NULL;

	this->create_output_folders();
}

Canvas::~Canvas() {
	delete[] this->pixel_color;
}

Color Canvas::get_pixel_color(const int row, const int column) {
	return this->pixel_color[row * this->width + column];
}

void Canvas::add_pixel_color(const int row, const int column, const Color& color) {
	this->pixel_color[row * this->width + column] += color;
}

void Canvas::set_pixel_color(const int row, const int column, const Color& color) {
	this->pixel_color[row * this->width + column] = color;
}

void Canvas::generate_image(const bool   is_final_image, 
							const int    samples_count, 
							const double seconds) {
	if (!is_final_image 
		&& this->output_struction_option > 0
		&& samples_count % this->output_struction_option == 0) {
		std::string filename = this->samples_image_prefix;
		std::stringstream ss_seconds;
		ss_seconds << std::setiosflags(std::ios::fixed) << std::setprecision(3) << seconds;

		filename += ("smp" + std::to_string(samples_count));
		filename += ("_sec" + ss_seconds.str());
		filename += ".bmp";

		this->generate_image(filename.c_str(), samples_count);
	} else if (is_final_image) {
		std::string filename = this->final_image_prefix;
		std::stringstream ss_seconds;
		ss_seconds << std::setiosflags(std::ios::fixed) << std::setprecision(3) << seconds;

		filename += ("_wid" + std::to_string(width));
		filename += ("_hgt" + std::to_string(height));
		filename += ("_acc" + std::to_string(config->get_SCENE_ACCELERATOR_OPTION()));
		filename += ("_smp" + std::to_string(samples_count));
		filename += ("_sec" + ss_seconds.str());
		filename += ".bmp";

		this->generate_image(filename.c_str(), samples_count);
	}
}

void Canvas::generate_image(const char* image_filepath, int samples_count) {
	FILE* file;
	errno_t error_code;
	error_code = fopen_s(&file, image_filepath, "wb");
	if (error_code != 0) {
		std::cout << "ERROR::BITMAP::FILE_NOT_EXIST" << std::endl;
	} else {
		// step1 Configurate headers of an bmp file.
		// -----------------------------------------
		Bitmap_File_Header       file_header;
		Bitmap_Infomation_Header info_header;
		const int size = width * height * 3;

		// Set Bitmap file header.
		file_header.fh_type               = 0x4D42;
		file_header.fh_file_size          = sizeof(Bitmap_File_Header) + sizeof(Bitmap_Infomation_Header) + size;
		file_header.fh_reserved1          = 0;
		file_header.fh_reserved2          = 0;
		file_header.fh_offset             = sizeof(Bitmap_File_Header) + sizeof(Bitmap_Infomation_Header);

		// Set Bitmap information header.
		info_header.ih_header_size        = sizeof(Bitmap_Infomation_Header);
		info_header.ih_image_width        = this->width;
		info_header.ih_image_height       = this->height;
		info_header.ih_planes             = 1;
		info_header.ih_bits_per_pixel     = 24;
		info_header.ih_compression        = 0;
		info_header.ih_image_size         = size;
		info_header.ih_X_pixels_per_meter = 0;
		info_header.ih_Y_pixels_per_meter = 0;
		info_header.ih_color_used         = 0;
		info_header.ih_color_important    = 0;

		// step2 Allocate storage for output_RGB.
		// --------------------------------------
		output_RGB = new unsigned char[size];

		// step3 Convert and store color information for each pixel.
		// ---------------------------------------------------------
		for (int row = 0; row < this->height; row++) {
			for (int column = 0; column < this->width; column++) {
				Color final_color = pixel_color[row * this->width + column];
				int index = (this->height - 1 - row) * this->width * 3 + column * 3;

				//Gamma Correction
				final_color.set_r(pow(final_color.get_r() / samples_count, 1 / gamma_correction_coefficient));
				final_color.set_g(pow(final_color.get_g() / samples_count, 1 / gamma_correction_coefficient));
				final_color.set_b(pow(final_color.get_b() / samples_count, 1 / gamma_correction_coefficient));

				// Store color information in array.
				final_color.clamp();
				output_RGB[index]     = static_cast<int>(255.999 * final_color.get_b());
				output_RGB[index + 1] = static_cast<int>(255.999 * final_color.get_g());
				output_RGB[index + 2] = static_cast<int>(255.999 * final_color.get_r());
			}
		}

		// step4 Output to bmp file.
		// -------------------------
		fwrite(&file_header, sizeof(Bitmap_File_Header), 1, file);
		fwrite(&info_header, sizeof(Bitmap_Infomation_Header), 1, file);
		fwrite(output_RGB, static_cast<size_t>(size), 1, file);

		// step5 Deallocate storage of output_RGB.
		// ---------------------------------
		delete[] output_RGB;

		fclose(file);
	}
}

void Canvas::create_output_folders() {
	if (this->output_struction_option == -1) {
		std::string command = "mkdir " + config->get_OUTPUT_FOLDER_PATH();
		std::system(command.c_str());
	} else if (this->output_struction_option == -2) {
		std::string command = "mkdir " + config->get_OUTPUT_FOLDER_PATH() + "\\" + config->get_OUTPUT_PROJECT_NAME();
		std::system(command.c_str());
	} else if (this->output_struction_option > 0){
		std::string command = "mkdir " + config->get_OUTPUT_FOLDER_PATH() + "\\" + config->get_OUTPUT_PROJECT_NAME();
		std::system(command.c_str());
		command += "\\samples";
		std::system(command.c_str());
	}
}
