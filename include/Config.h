#pragma once

#include <map>

#include "Color.h"
#include "Point3.h"

class Config {
public:
	Config(const char* config_filepath = "config.txt");
	~Config();

	// Getters
	// -------
	/* MULTITHREADS*/
	int         get_MULTITHREADS_NUMBER_OF_THREADS()               const;

	/* IMAGE */
	int         get_IMAGE_WIDTH()                                  const;
	int         get_IMAGE_HEIGHT()                                 const;
	int         get_IMAGE_SAMPLES_PER_PIXEL()                      const;
	int         get_IMAGE_PRESET_IMAGE_OPTION()                    const;

	/* SCENE */
	int         get_SCENE_PRESET_SCENE_OPTION()                    const;
	int         get_SCENE_MAX_RECURSION_DEPTH()                    const;
	Color       get_SCENE_AMBIENT_COLOR()                          const;

	/* SCENE_ACCELERATOR */
	int         get_SCENE_ACCELERATOR_OPTION()                     const;
	int         get_SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE() const;

	/* CAMERA */
	int         get_CAMERA_PRESET_CAMERA_OPTION()                  const;

	Point3      get_CAMERA_POSITION()                              const;
	Point3      get_CAMERA_PLACE_OF_GAZE()                         const;
	Vector3     get_CAMERA_UP_DIRECTION()                          const;

	double      get_CAMERA_VERTICAL_FIELD_OF_VIEW()                const;
	double      get_CAMERA_APERTURE()                              const;
	double      get_CAMERA_ASPECT_RATIO()                          const;
	double      get_CAMERA_FOCAL_LENGTH()                          const;
	double      get_CAMERA_MINIMUM_VIEWING_DISTANCE()              const;

	/* RENDERER */
	int         get_RENDERER_OPTION()                              const;
	int         get_RENDERER_TRANSMISSION_TRANSFORMATION_METHOD()  const;

	/* OUTPUT */
	double      get_OUTPUT_GAMMA_CORRECTION_COEFFICIENT()          const;

	int         get_OUTPUT_STRUCTURE_OPTION()                      const;
	std::string get_OUTPUT_FOLDER_PATH()                           const;
	std::string get_OUTPUT_PROJECT_NAME()                          const;

private:
	void trim(std::string& string);
	void read_config_file(const char* config_filepath);
	bool analyse_line(std::string& line, std::string& key, std::string& value);
	
private:
	std::map<std::string, std::string> map;

	/* MULTITHREADS*/
	int         MULTITHREADS_NUMBER_OF_THREADS;

	/* IMAGE */
	int         IMAGE_WIDTH;
	int         IMAGE_HEIGHT;
	int         IMAGE_SAMPLES_PER_PIXEL;
	int         IMAGE_PRESET_IMAGE_OPTION;

	/* SCENE */
	int         SCENE_PRESET_SCENE_OPTION;
	int         SCENE_MAX_RECURSION_DEPTH;
	Color       SCENE_AMBIENT_COLOR;

	/* SCENE_ACCELERATOR */
	int         SCENE_ACCELERATOR_OPTION;
	int         SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE;

	/* CAMERA */
	int         CAMERA_PRESET_CAMERA_OPTION;
	Point3      CAMERA_POSITION;
	Point3      CAMERA_PLACE_OF_GAZE;
	Vector3     CAMERA_UP_DIRECTION;
	double      CAMERA_VERTICAL_FIELD_OF_VIEW;
	double      CAMERA_APERTURE;
	double      CAMERA_ASPECT_RATIO;
	double      CAMERA_FOCAL_LENGTH;
	double      CAMERA_MINIMUM_VIEWING_DISTANCE;

	/* RENDERER */
	int         RENDERER_OPTION;
	int         RENDERER_RENDER_ALGORITHM;
	int         RENDERER_TRANSMISSION_CALCULATION_METHOD;

	int         RENDERER_TRANSMISSION_TRANSFORMATION_EQUATION;

	/* OUTPUT */
	double      OUTPUT_GAMMA_CORRECTION_COEFFICIENT;

	int         OUTPUT_STRUCTURE_OPTION;
	std::string OUTPUT_FOLDER_PATH;
	std::string OUTPUT_PROJECT_NAME;
};