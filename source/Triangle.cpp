#include "../include/Triangle.h"

Triangle::Triangle(const Point3   v1, const Point3   v2, const Point3   v3,
				   const Point3  vt1, const Point3  vt2, const Point3  vt3,
				   const Vector3 vn1, const Vector3 vn2, const Vector3 vn3,
                   std::shared_ptr<Material> material,
                   std::shared_ptr<Texture> texture) {
	this->vertices[0] = v1;
	this->vertices[1] = v2;
	this->vertices[2] = v3;

	this->texture_vertices[0] = vt1;
	this->texture_vertices[1] = vt2;
	this->texture_vertices[2] = vt3;

    this->normal = ((vn1 + vn2 + vn3) / 3).get_unit_vector();

    this->material = material;
    this->texture = texture;
    compute_AABB();
}

Triangle::Triangle(const Point3  v1, const Point3  v2, const Point3  v3,
                   const Point3 vt1, const Point3 vt2, const Point3 vt3,
                   std::shared_ptr<Material> material,
                   std::shared_ptr<Texture> texture) {
    // Don't judge which face is in the front.
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;

    this->texture_vertices[0] = vt1;
    this->texture_vertices[1] = vt2;
    this->texture_vertices[2] = vt3;

    this->normal = cross_product(v2 - v1, v3 - v1).get_unit_vector();

    this->material = material;
    this->texture = texture;
    compute_AABB();
}

Triangle::Triangle(const Point3 v1, const Point3 v2, const Point3 v3,
                   std::shared_ptr<Material> material,
                   std::shared_ptr<Texture> texture) {

    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;


    this->texture_vertices[0] = Point3(0.0, 0.0, 0.0);
    this->texture_vertices[1] = Point3(1.0, 0.0, 0.0);
    this->texture_vertices[2] = Point3(0.0, 1.0, 0.0);

    this->normal = cross_product(v2 - v1, v3 - v1).get_unit_vector();

    this->material = material;
    this->texture = texture;
    compute_AABB();
}

Triangle::~Triangle() {
}


Point3 Triangle::get_reference_point() const {
	return Point3((vertices[0][0] + vertices[0][1] + vertices[0][2]) / 3,
				  (vertices[1][0] + vertices[1][1] + vertices[1][2]) / 3,
				  (vertices[2][0] + vertices[2][1] + vertices[2][2]) / 3);
}

bool Triangle::hit(const Ray& ray, Hit_Record& record) const {
    Point3  P1 = this->vertices[0], 
            P2 = this->vertices[1], 
            P3 = this->vertices[2];
    Point3  O  = ray.get_origin();
    Vector3 D  = ray.get_direction();

    Vector3 E1 = P2 - P1,
            E2 = P3 - P1;
    Vector3 S  = O - P1,
            S1 = cross_product(D, E2),
            S2 = cross_product(S, E1);

    double S1_dot_E1_inv = 1.0 / dot_product(S1, E1);

    double t     = S1_dot_E1_inv * dot_product(S2, E2);
    double beta  = S1_dot_E1_inv * dot_product(S1, S);
    double gamma = S1_dot_E1_inv * dot_product(S2, D);
    double alpha = 1 - beta - gamma;

    if (alpha < TOLERANCE || beta < TOLERANCE || gamma < TOLERANCE) {
        return false;
    }

    if (t < TOLERANCE || t > record.distance) {
        return false;
    }

    record.distance      = t;
    record.point         = ray.at(t);
    record.texture_point = this->texture_vertices[0]
                         + beta  * (this->texture_vertices[1] - this->texture_vertices[0])
                         + gamma * (this->texture_vertices[2] - this->texture_vertices[0]);
    record.front_face    = dot_product(this->normal, D) < 0;
    record.normal        = record.front_face ? this->normal : -this->normal;
    record.material      = this->material;
    record.texture       = this->texture;
    record.light_pass_proportion *= 0.5 * this->material->get_refraction_coefficient() * record.texture->albedo(record.texture_point);

    return true;
}

void Triangle::compute_AABB() {
    this->aabb = AABB(Point3(fmin(vertices[0][0], fmin(vertices[1][0], vertices[2][0])),
                             fmin(vertices[0][1], fmin(vertices[1][1], vertices[2][1])),
                             fmin(vertices[0][2], fmin(vertices[1][2], vertices[2][2]))),
                      Point3(fmax(vertices[0][0], fmax(vertices[1][0], vertices[2][0])),
                             fmax(vertices[0][1], fmax(vertices[1][1], vertices[2][1])),
                             fmax(vertices[0][2], fmax(vertices[1][2], vertices[2][2]))));
    this->aabb.avoid_plane();
}
