#pragma once

#include "Config.h"
#include "constants.h"
#include "Point3.h"
#include "Ray.h"
#include "Vector3.h"

class Camera {
public:
    // Constructors and Destructor.
    // ----------------------------
    Camera(std::shared_ptr<Config> config);
    ~Camera();

    // Overloaded operators.
    // ---------------------
    Camera& operator=(const Camera camera);

    // Other functions.
    // ----------------
    void build_camera(Point3 position, Point3 place_of_gaze, Vector3 up_direction,
                      double vfov, double aspect_ratio, double aperture, double focal_length);
    Ray get_ray(const int row, const int column) const;

private:
    std::shared_ptr<Config> config;

    Point3  position;

    Point3  upper_left_corner;
    Vector3 horizontally_right_on_viewport;
    Vector3 vertically_down_on_viewport;

    Vector3 u, v, w;

    double  lens_radius;
    double  focal_length;
};