#pragma once

#include <memory>
#include <vector>

#include "BVH_Node.h"
#include "Color.h"
#include "utilities.h"
#include "Config.h"
#include "constants.h"
#include "Hit_Record.h"
#include "Lightsource.h"
#include "OBJ_Loader.h"
#include "Point3.h"
#include "Primitive.h"
#include "Ray.h"
#include "Texture.h"
#include "Vector3.h"

class Scene {
public:
	// Constructors and Destructor
	// ---------------------------
	Scene(std::shared_ptr<Config> config);
	~Scene();

	// Other functions
	// ---------------
	Color ray_color(int renderer_type, Ray& ray);

private:
	void build_scene(int option);
	void add_primitive(std::shared_ptr<Primitive> primitive);

	std::shared_ptr<BVH_Node> build_middle_BVH_tree(const int l, const int r);
	std::shared_ptr<BVH_Node> build_equal_count_BVH_tree(const int l, const int r);
	std::shared_ptr<BVH_Node> build_SAH_BVH_tree(const int l, const int r);

	double specular_proportion(const double ratio_of_refractive_indices,
							   const double sin_in,
							   const double cos_in,
							   const double sin_out,
							   const double cos_out) const;
	bool hit_lightsource(Ray& ray, Hit_Record& record) const;
	Color Lambert_diffuse_model(const Ray& ray, Hit_Record& record) const;

	Color ambient_model                           (const Ray& ray, int recursion_depth) const;
	Color Lambert_diffuse_model                   (const Ray& ray, int recursion_depth) const;
	Color Phong_modle                             (const Ray& ray, int recursion_depth) const;
	Color Blinn_Phong_model                       (const Ray& ray, int recursion_depth) const;
	Color simplified_Whitted_ray_trace_by_weight  (const Ray& ray, int recursion_depth) const;
	Color simplified_Whitted_ray_trace_Monte_Carlo(const Ray& ray, int recursion_depth) const;
	Color Whitted_ray_trace_by_weight             (const Ray& ray, int recursion_depth) const;
	Color Whitted_ray_trace_Monte_Carlo           (const Ray& ray, int recursion_depth) const;
	Color optimized_ray_trace_by_weight           (const Ray& ray, int recursion_depth) const;
	Color optimized_ray_trace_Monte_Carlo         (const Ray& ray, int recursion_depth) const;


	bool hit            (const std::shared_ptr<BVH_Node> node, const Ray& ray, Hit_Record& record) const;
	bool hit_brute_force(                                      const Ray& ray, Hit_Record& record) const;
	bool hit_BVH        (const std::shared_ptr<BVH_Node> node, const Ray& ray, Hit_Record& record) const;

private:
	std::shared_ptr<Config>                   config;

	int                                       accelerator;
	int                                       maximum_primitives_in_node;
	int                                       max_recursion_depth;
	Color                                     ambient_color;

	std::vector<std::shared_ptr<Primitive>>   primitives;
	std::vector<std::shared_ptr<Lightsource>> lightsources;

	std::shared_ptr<BVH_Node>                 BVH_tree;
};