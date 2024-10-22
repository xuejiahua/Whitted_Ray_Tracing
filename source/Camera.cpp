#include "../include/Camera.h"
#include "../include/utilities.h"

Camera::Camera(std::shared_ptr<Config> config) {
	this->config = config;
	switch (config->get_CAMERA_PRESET_CAMERA_OPTION()) {
		default:
		case 0:
			build_camera(config->get_CAMERA_POSITION(),
						 config->get_CAMERA_PLACE_OF_GAZE(),
						 config->get_CAMERA_UP_DIRECTION(),
						 config->get_CAMERA_VERTICAL_FIELD_OF_VIEW(),
						 config->get_CAMERA_ASPECT_RATIO(),
						 config->get_CAMERA_APERTURE(),
						 config->get_CAMERA_FOCAL_LENGTH());
			break;

		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			build_camera(Point3(278, 278, -800),
						 Point3(278, 278, 0.0),
						 Vector3(0.0, 1.0, 0.0),
						 40,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;

		case 31:
		case 32:
			build_camera(Point3(0.0, 0.0, 0.0),
						 Point3(0.0, 0.0, -8.0),
						 Vector3(0.0, 1.0, 0.0),
						 60,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;

		case 33:
			build_camera(Point3(478, 278, -600),
						 Point3(278, 278, 0),
						 Vector3(0.0, 1.0, 0.0),
						 40,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;


		case 34:
			build_camera(Point3(-12.3693, 2.0, 5),
						 Point3(0.0, 0.0, 0.0),
						 Vector3(0.0, 1.0, 0.0),
						 20,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;

		case 35:
			build_camera(Point3(-4.0, 2.0, -17.33333),
						 Point3(0.0, 0.0, 0.0),
						 Vector3(0.0, 1.0, 0.0),
						 20,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;
		case 36:
			build_camera(Point3(0.0, 20.0, -0.1),
						 Point3(0.0, 0.0, 0.0),
						 Vector3(0.0, 1.0, 0.0),
						 20,
						 config->get_CAMERA_ASPECT_RATIO(),
						 0.0,
						 1.0);
			break;
	}
}

Camera::~Camera() {
}

Camera& Camera::operator=(const Camera camera) {
	this->position                       = camera.position;
	this->upper_left_corner              = camera.upper_left_corner;
	this->horizontally_right_on_viewport = camera.horizontally_right_on_viewport;
	this->vertically_down_on_viewport    = camera.vertically_down_on_viewport;
	this->u                              = camera.u;
	this->v                              = camera.v;
	this->w                              = camera.w;
	this->lens_radius                    = camera.lens_radius;
	this->focal_length                   = camera.focal_length;

	return *this;
}

void Camera::build_camera(Point3 position, Point3 place_of_gaze, Vector3 up_direction, 
						  double vfov, double aspect_ratio, double aperture, double focal_length) {
	this->position     = position;
	this->lens_radius  = aperture;
	this->focal_length = focal_length;

	double viewport_height = 2.0 * tan(vfov * PI / 180 / 2);
	double viewport_width  = aspect_ratio * viewport_height;

	this->w = (position - place_of_gaze).get_unit_vector();
	this->u = cross_product(up_direction, w).get_unit_vector();
	this->v = cross_product(w, u);

	this->horizontally_right_on_viewport = focal_length * viewport_width * u;
	this->vertically_down_on_viewport    = focal_length * viewport_height * (-v);
	this->upper_left_corner = position - focal_length * w
		- vertically_down_on_viewport / 2
		- horizontally_right_on_viewport / 2;
}

Ray Camera::get_ray(const int row, const int column) const {
	double s = (row    + random_double()) / config->get_IMAGE_HEIGHT();
	double t = (column + random_double()) / config->get_IMAGE_WIDTH();

	Vector3 offset(0.0, 0.0, 0.0);
	if (this->lens_radius > TOLERANCE) {
		offset = random_vector_in_disk(this->lens_radius);
		offset = offset.get_x() * this->u + offset.get_y() * this->v;
	}

	return Ray(position + offset,
			   upper_left_corner + s * vertically_down_on_viewport
			                     + t * horizontally_right_on_viewport - position - offset);
}