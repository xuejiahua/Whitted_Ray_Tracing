#include "../include/Config.h"

#include <fstream>
#include <iostream>
#include <omp.h>
#include <string>

Config::Config(const char* config_filepath) {
	read_config_file(config_filepath);

	/* MULTITHREADS */
	this->MULTITHREADS_NUMBER_OF_THREADS      = std::stoi(map[std::string("MULTITHREADS_NUMBER_OF_THREADS")]);
	if (this->MULTITHREADS_NUMBER_OF_THREADS <= 0) {
		this->MULTITHREADS_NUMBER_OF_THREADS = 2 * omp_get_num_procs() - 1;
	}

	/* IMAGE */
	this->IMAGE_WIDTH                         = std::stoi(map[std::string("IMAGE_WIDTH")]);
	if (this->IMAGE_WIDTH % 4 != 0) {
		this->IMAGE_WIDTH += 4 - this->IMAGE_WIDTH % 4;
	}
	this->IMAGE_HEIGHT                        = std::stoi(map[std::string("IMAGE_HEIGHT")]);
	this->IMAGE_SAMPLES_PER_PIXEL             = std::stoi(map[std::string("IMAGE_SAMPLES_PER_PIXEL")]);
	this->IMAGE_PRESET_IMAGE_OPTION           = std::stoi(map[std::string("IMAGE_PRESET_IMAGE_OPTION")]);

	/* SCENE */
	if (std::stoi(map[std::string("IMAGE_PRESET_IMAGE_OPTION")]) != 0) {
		this->SCENE_PRESET_SCENE_OPTION       = std::stoi(map[std::string("IMAGE_PRESET_IMAGE_OPTION")]);
	} else if (std::stoi(map[std::string("SCENE_PRESET_SCENE_OPTION")]) != 0){
		this->SCENE_PRESET_SCENE_OPTION       = std::stoi(map[std::string("SCENE_PRESET_SCENE_OPTION")]);
	} else {
		this->SCENE_PRESET_SCENE_OPTION       = 1;
	}
	this->SCENE_MAX_RECURSION_DEPTH           = std::stoi(map[std::string("SCENE_MAX_RECURSION_DEPTH")]);
	this->SCENE_AMBIENT_COLOR                 = Color(std::stod(map[std::string("SCENE_AMBIENT_COLOR_R")]),
									                  std::stod(map[std::string("SCENE_AMBIENT_COLOR_G")]),
									                  std::stod(map[std::string("SCENE_AMBIENT_COLOR_B")]));
	
	/* SCENE_ACCELERATOR */
	if (map[std::string("SCENE_ACCELERATOR")] == std::string("brute_force")) {
		this->SCENE_ACCELERATOR_OPTION        = 0;
	} else if (map[std::string("SCENE_ACCELERATOR")] == std::string("middle_BVH")) {
		this->SCENE_ACCELERATOR_OPTION        = 1;
	} else if (map[std::string("SCENE_ACCELERATOR")] == std::string("equal_count_BVH")) {
		this->SCENE_ACCELERATOR_OPTION        = 2;
	} else {
	    // map[std::string("SCENE_ACCELERATOR")] == std::string("SAH_BVH")
		this->SCENE_ACCELERATOR_OPTION        = 3;
	}

	this->SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE 
		                                      = std::stoi(map[std::string("SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE")]);

	/* CAMERA */
	if (std::stoi(map[std::string("CAMERA_PRESET_CAMERA_OPTION")]) == -1) {
		this->CAMERA_PRESET_CAMERA_OPTION     = 0;
	} else if (std::stoi(map[std::string("IMAGE_PRESET_IMAGE_OPTION")]) != 0) {
		this->CAMERA_PRESET_CAMERA_OPTION     = std::stoi(map[std::string("IMAGE_PRESET_IMAGE_OPTION")]);
	} else {
		this->CAMERA_PRESET_CAMERA_OPTION     = std::stoi(map[std::string("CAMERA_PRESET_CAMERA_OPTION")]);
	}
	
	this->CAMERA_POSITION                     = Point3(std::stod(map[std::string("CAMERA_POSITION_X")]),
								                       std::stod(map[std::string("CAMERA_POSITION_Y")]),
								                       std::stod(map[std::string("CAMERA_POSITION_Z")]));
	this->CAMERA_PLACE_OF_GAZE                = Point3(std::stod(map[std::string("CAMERA_PLACE_OF_GAZE_X")]),
	                                                   std::stod(map[std::string("CAMERA_PLACE_OF_GAZE_Y")]),
	                                                   std::stod(map[std::string("CAMERA_PLACE_OF_GAZE_Z")]));
	this->CAMERA_UP_DIRECTION                 = Vector3(std::stod(map[std::string("CAMERA_UP_DIRECTION_X")]),
	                                                    std::stod(map[std::string("CAMERA_UP_DIRECTION_Y")]),
	                                                    std::stod(map[std::string("CAMERA_UP_DIRECTION_Z")]));
	this->CAMERA_ASPECT_RATIO                 = double(this->IMAGE_WIDTH) / this->IMAGE_HEIGHT;
	this->CAMERA_VERTICAL_FIELD_OF_VIEW       = std::stod(map[std::string("CAMERA_VERTICAL_FIELD_OF_VIEW")]);
	this->CAMERA_VERTICAL_FIELD_OF_VIEW       = this->CAMERA_VERTICAL_FIELD_OF_VIEW > 179.0 ? this->CAMERA_VERTICAL_FIELD_OF_VIEW = 179.0 :
		                                        this->CAMERA_VERTICAL_FIELD_OF_VIEW < 1.0 ?   this->CAMERA_VERTICAL_FIELD_OF_VIEW = 1.0 :
		                                                                                      this->CAMERA_VERTICAL_FIELD_OF_VIEW;
	this->CAMERA_APERTURE                     = std::stod(map[std::string("CAMERA_APERTURE")]);
	this->CAMERA_FOCAL_LENGTH                 = std::stod(map[std::string("CAMERA_FOCAL_LENGTH")]);
	this->CAMERA_MINIMUM_VIEWING_DISTANCE     = std::stod(map[std::string("CAMERA_MINIMUM_VIEWING_DISTANCE")]);
	
	/* RENDERER */
	if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("ambient_model")) {
		this->RENDERER_RENDER_ALGORITHM = 1;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("Lambert_diffuse_model")) {
		this->RENDERER_RENDER_ALGORITHM = 2;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("Phong_model")) {
		this->RENDERER_RENDER_ALGORITHM = 3;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("Blinn_Phong_model")) {
		this->RENDERER_RENDER_ALGORITHM = 4;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("simplified_Whitted_ray_trace")) {
		this->RENDERER_RENDER_ALGORITHM = 5;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("Whitted_ray_trace")) {
		this->RENDERER_RENDER_ALGORITHM = 6;
	} else if (map[std::string("RENDERER_RENDER_ALGORITHM")] == std::string("optimized_ray_trace")){
		this->RENDERER_RENDER_ALGORITHM = 7;
	} else {
		this->RENDERER_RENDER_ALGORITHM = 6;
	}

	if (map[std::string("RENDERER_TRANSMISSION_CALCULATION_METHOD")] == std::string("by_weight")) {
		this->RENDERER_TRANSMISSION_CALCULATION_METHOD = 1;
	} else if (map[std::string("RENDERER_TRANSMISSION_CALCULATION_METHOD")] == std::string("Monte_Carlo")) {
		this->RENDERER_TRANSMISSION_CALCULATION_METHOD = 2;
	} else {
		this->RENDERER_TRANSMISSION_CALCULATION_METHOD = 2;
	}

	switch (this->RENDERER_RENDER_ALGORITHM) {
		case 1:
		case 2:
		case 3:
		case 4:
			this->RENDERER_OPTION = this->RENDERER_RENDER_ALGORITHM * 10;
			break;
		case 5:
		case 6:
		case 7:
			this->RENDERER_OPTION = this->RENDERER_RENDER_ALGORITHM * 10 + this->RENDERER_TRANSMISSION_CALCULATION_METHOD;
			break;
	}

	if (map[std::string("RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION")] == std::string("Fresnel_equation")) {
		this->RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION = 1;
	} else if (map[std::string("RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION")] == std::string("Schlick_approximation")){
		this->RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION = 2;
	} else {
		this->RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION = 1;
	}

	/* OUTPUT */
	this->OUTPUT_GAMMA_CORRECTION_COEFFICIENT = std::stod(map[std::string("OUTPUT_GAMMA_CORRECTION_COEFFICIENT")]);

	if (map[std::string("OUTPUT_STRUCTURE")] == std::string("only_final_image")) {
		this->OUTPUT_STRUCTURE_OPTION         = -1;
	} else if (map[std::string("OUTPUT_STRUCTURE")] == std::string("final_image_in_folder")) {
		this->OUTPUT_STRUCTURE_OPTION         = -2;
	} else {
		// map[std::string("OUTPUT_STRUCTURE")] == std::string("final_image_and_sample_images_in_folder")
		this->OUTPUT_STRUCTURE_OPTION         = std::stod(map[std::string("OUTPUT_STRUCTURE_SAMPLE_INTERVAL")]);
		this->OUTPUT_STRUCTURE_OPTION         = this->OUTPUT_STRUCTURE_OPTION < 1 ? 1 : this->OUTPUT_STRUCTURE_OPTION;
	}
	this->OUTPUT_FOLDER_PATH                  = map[std::string("OUTPUT_FOLDER_PATH")];
	this->OUTPUT_PROJECT_NAME                 = map[std::string("OUTPUT_PROJECT_NAME")];
}

Config::~Config() {
}

void Config::trim(std::string& string) {
	string = string.substr(string.find_first_not_of(" \n\r\t\f\v"),
				           string.find_last_not_of (" \n\r\t\f\v") + 1);
}

void Config::read_config_file(const char* config_filepath) {
	map.clear();
	std::ifstream config_file(config_filepath);
	if (!config_file) {
		std::cout << "ERROR::CONFIG FILE " << config_filepath << " CANNOT BE OPEN." << std::endl;
		exit(-1);
	} else {
		std::string line, key, value;
		while (std::getline(config_file, line)) {
			if (analyse_line(line, key, value)) {
				map[key] = value;
			}
		}
	}
	config_file.close();
}

bool Config::analyse_line(std::string& line, std::string& key, std::string& value) {
	if (line.empty())
		return false;

	int start = 0, end = line.size() - 1, pos = line.find("=");
	if ((line.find("#") != -1) || (line.find("*") != -1) || pos == -1) {
		return false;
	}

	key = line.substr(0, pos);
	value = line.substr(pos + 1, end + 1 - (pos + 1));
	trim(key);
	trim(value);

	if (key.empty()) {
		return false;
	}
	return true;
}

int Config::get_MULTITHREADS_NUMBER_OF_THREADS() const {
	return this->MULTITHREADS_NUMBER_OF_THREADS;
}

int Config::get_IMAGE_WIDTH() const {
	return this->IMAGE_WIDTH;
}

int Config::get_IMAGE_HEIGHT() const {
	return this->IMAGE_HEIGHT;
}

int Config::get_IMAGE_SAMPLES_PER_PIXEL() const {
	return this->IMAGE_SAMPLES_PER_PIXEL;
}

int Config::get_IMAGE_PRESET_IMAGE_OPTION() const {
	return this->IMAGE_PRESET_IMAGE_OPTION;
}

int Config::get_SCENE_ACCELERATOR_OPTION() const {
	return this->SCENE_ACCELERATOR_OPTION;
}

int Config::get_SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE() const {
	return this->SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE;
}

int Config::get_SCENE_PRESET_SCENE_OPTION() const {
	return this->SCENE_PRESET_SCENE_OPTION;
}

int Config::get_SCENE_MAX_RECURSION_DEPTH() const {
	return this->SCENE_MAX_RECURSION_DEPTH;
}

Color Config::get_SCENE_AMBIENT_COLOR() const {
	return this->SCENE_AMBIENT_COLOR;
}

int Config::get_CAMERA_PRESET_CAMERA_OPTION() const {
	return this->CAMERA_PRESET_CAMERA_OPTION;
}

Point3 Config::get_CAMERA_POSITION() const {
	return this->CAMERA_POSITION;
}

Point3 Config::get_CAMERA_PLACE_OF_GAZE() const {
	return this->CAMERA_PLACE_OF_GAZE;
}

Vector3 Config::get_CAMERA_UP_DIRECTION() const {
	return this->CAMERA_UP_DIRECTION;
}

double Config::get_CAMERA_VERTICAL_FIELD_OF_VIEW() const {
	return this->CAMERA_VERTICAL_FIELD_OF_VIEW;
}

double Config::get_CAMERA_APERTURE() const {
	return this->CAMERA_APERTURE;
}

double Config::get_CAMERA_ASPECT_RATIO() const {
	return this->CAMERA_ASPECT_RATIO;
}

double Config::get_CAMERA_FOCAL_LENGTH() const {
	return this->CAMERA_FOCAL_LENGTH;
}

double Config::get_CAMERA_MINIMUM_VIEWING_DISTANCE() const {
	return this->CAMERA_MINIMUM_VIEWING_DISTANCE;
}

int Config::get_RENDERER_OPTION() const {
	return this->RENDERER_OPTION;
}

int Config::get_RENDERER_TRANSMISSION_TRANSFORMATION_METHOD() const {
	return this->RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION;
}

double Config::get_OUTPUT_GAMMA_CORRECTION_COEFFICIENT() const {
	return this->OUTPUT_GAMMA_CORRECTION_COEFFICIENT;
}

int Config::get_OUTPUT_STRUCTURE_OPTION() const {
	return this->OUTPUT_STRUCTURE_OPTION;
}

std::string Config::get_OUTPUT_FOLDER_PATH() const {
	return this->OUTPUT_FOLDER_PATH;
}

std::string Config::get_OUTPUT_PROJECT_NAME() const {
	return this->OUTPUT_PROJECT_NAME;
}