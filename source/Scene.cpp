#include "../include/Scene.h"

#include <algorithm>

#include "../include/Sphere.h"
#include "../include/Ray.h"

Scene::Scene(std::shared_ptr<Config> config) {
	this->config                     = config;
	this->max_recursion_depth        = config->get_SCENE_MAX_RECURSION_DEPTH();
	this->ambient_color              = config->get_SCENE_AMBIENT_COLOR();
	this->accelerator                = config->get_SCENE_ACCELERATOR_OPTION();
	this->maximum_primitives_in_node = config->get_SCENE_ACCELERATOR_MAXIMUM_PRIMITIVES_IN_NODE();

	this->build_scene(config->get_IMAGE_PRESET_IMAGE_OPTION());
	this->BVH_tree = accelerator == 0 ? NULL : 
		             accelerator == 1 ? this->build_middle_BVH_tree(0, int(this->primitives.size())) :
		             accelerator == 2 ? this->build_equal_count_BVH_tree(0, int(this->primitives.size())) :
		                                this->build_SAH_BVH_tree(0, int(this->primitives.size()));
}

Scene::~Scene() {
	this->primitives.clear();
	this->BVH_tree = NULL;
}

void Scene::build_scene(int option) {
	this->primitives.clear();

	switch (option) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6: {
			auto material_matte = std::make_shared<Material>(0.05, 0.50, 0.0, 0.0, 0.0, 0.0, 120, 0.0);
			auto material_light = std::make_shared<Material>(0.00, 0.00, 0.0, 0.0, 0.0, 0.0, 120, 25.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_bunny  = std::make_shared<Texture>(1, Color(0.725, 0.71, 0.68));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray   = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_matte, texture_gray);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white);
			obj_loader->read_obj("resource/Stanford_bunny.obj", 0,
								 material_matte, texture_bunny,
								 1600, 1600, 1600,
								 0.0, 0.0,
								 0.0, 0.0,
								 0.0, 0.0,
								 0.0, 0.0, 0.0,
								 false, true, false,
								 false, false, false,
								 250, -50.0, 220.0);
			break;
		}

		case 11: {
			auto material_matte = std::make_shared<Material>(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 30, 0.0);
			auto material_ceil  = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.225, 0.21, 0.18));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(277.0, 553.0, 220.0), 800000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_ceil,  texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_matte, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_matte, texture_box);
			break;
		}

		case 12:
		case 13:
		case 14: {
			auto material_matte = std::make_shared<Material>(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 30, 0.0);
			auto material_ceil  = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(277.0, 553.0, 220.0), 800000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_ceil,  texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_matte, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_matte, texture_box);
			break;
		}

		case 15: {
			auto material_matte  = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 30, 0.0);
			auto material_ceil   = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_mirror = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white   = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box     = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red     = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green   = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1   = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2   = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(277.0, 553.0, 220.0), 800000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_ceil,  texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_mirror, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_mirror, texture_box);
			break;
		}

		case 16: {
			auto material_matte = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 30, 0.0);
			auto material_ceil  = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, 0.0, 1.5, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(277.0, 553.0, 220.0), 800000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_ceil,  texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_glass, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_glass, texture_box);
			break;
		}

		case 17: {
			auto material_matte = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 30, 0.0);
			auto material_ceil  = std::make_shared<Material>(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(277.0, 553.0, 220.0), 800000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_ceil,  texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_matte, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_matte, texture_box);
			break;
		}

		case 18: {
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_light = std::make_shared<Material>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_matte, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_matte, texture_box);
			break;
		}

		case 19: {
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_light = std::make_shared<Material>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0);
			auto material_mirror = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_mirror, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_mirror, texture_box);
			break;
		}

		case 20: {
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_light = std::make_shared<Material>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, 0.0, 1.5, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box    = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_red    = std::make_shared<Texture>(1, Color(0.65, 0.05, 0.05));
			auto texture_green  = std::make_shared<Texture>(1, Color(0.12, 0.45, 0.15));
			auto texture_gray1  = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2  = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_red);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_green);
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white);
			obj_loader->read_obj("resource/boxes/tall_box.obj", 0,
								 material_glass, texture_box);
			obj_loader->read_obj("resource/boxes/short_box.obj", 0,
								 material_glass, texture_box);
			break;
		}

		case 21:
		{
			auto material_light  = std::make_shared<Material>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 25.0);
			auto material_matte  = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass  = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, 0.0, 1.5, 0.0, 0.0);
			auto material_mirror = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_white     = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_box       = std::make_shared<Texture>(1, Color(0.525, 0.51, 0.48));
			auto texture_cyan_blue = std::make_shared<Texture>(1, Color(0.484375, 0.8359375, 0.80859375));
			auto texture_purple    = std::make_shared<Texture>(1, Color(0.609375, 0.52734375, 0.8046875));
			auto texture_yellow    = std::make_shared<Texture>(1, Color(0.96875, 0.79296875, 0.49609375));
			auto texture_gray1     = std::make_shared<Texture>(1, Color(0.73, 0.73, 0.73));
			auto texture_gray2     = std::make_shared<Texture>(1, Color(0.65, 0.65, 0.65));

			this->add_primitive(std::make_shared<Sphere>(Point3(100, 100, 455), 100, material_mirror, texture_white));
			this->add_primitive(std::make_shared<Sphere>(Point3(455, 100, 455), 100, material_matte, texture_yellow));
			this->add_primitive(std::make_shared<Sphere>(Point3(455, 100, 100), 100, material_glass, texture_white));


			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/floor.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/wall.obj", 0,
								 material_matte, texture_gray2);
			obj_loader->read_obj("resource/Cornell_box/ceil.obj", 0,
								 material_matte, texture_gray1);
			obj_loader->read_obj("resource/Cornell_box/left.obj", 0,
								 material_matte, texture_cyan_blue);
			obj_loader->read_obj("resource/Cornell_box/right.obj", 0,
								 material_matte, texture_purple);
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white);

			break;
		}

		case 31: {
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 0.7, 0.0, 1.5, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_sphere = std::make_shared<Texture>(1, Color(0.6, 0.7, 0.8));
			auto texture_check  = std::make_shared<Texture>(2, 5, Color(0.85, 0.10, 0.10), Color(0.85, 0.85, 0.10));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(-20, 70, 20), 25000.0));
			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(30, 50, -12), 25000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			this->add_primitive(std::make_shared<Sphere>(Point3(-1.0, 0.0, -12.0), 2.0, material_matte, texture_sphere));
			this->add_primitive(std::make_shared<Sphere>(Point3(0.5, -0.5, -8.0),  1.5, material_glass, texture_white));
			obj_loader->read_obj("resource/square.obj", 1,
								 material_matte, texture_check);
			break;
		}

		case 32:
		{
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 0.7, 0.0, 1.5, 0.0, 0.0);

			auto texture_white = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_sphere = std::make_shared<Texture>(1, Color(0.6, 0.7, 0.8));
			auto texture_check = std::make_shared<Texture>(2, 5, Color(0.85, 0.10, 0.10), Color(0.85, 0.85, 0.10));

			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(-20, 70, 20), 25000.0));
			this->lightsources.push_back(std::make_shared<Lightsource>(Color(1.0, 1.0, 1.0), Point3(30, 50, -12), 25000.0));

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			this->add_primitive(std::make_shared<Sphere>(Point3(-1.0, 0.0, -12.0), 2.0, material_matte, texture_sphere));
			this->add_primitive(std::make_shared<Sphere>(Point3(0.5, -0.5, -8.0), 1.5, material_glass, texture_white));
			this->add_primitive(std::make_shared<Sphere>(Point3(0.5, -0.5, -8.0), -1.4, material_glass, texture_white));
			obj_loader->read_obj("resource/square.obj", 1,
								 material_matte, texture_check);
			break;
		}

		case 33: {
			auto material_light = std::make_shared<Material>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0);
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 0.8, 0.0, 1.5, 0.0, 0.0);
			auto material_metal = std::make_shared<Material>(0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_metal_1 = std::make_shared<Material>(0.0, 0.0, 0.8, 0.0, 0.5, 0.0, 0.0, 0.0);
			auto material_metal_2 = std::make_shared<Material>(0.0, 0.0, 0.8, 0.0, 1.0, 0.0, 0.0, 0.0);

			auto texture_white  = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			auto texture_boxes  = std::make_shared<Texture>(1, Color(0.48, 0.83, 0.53));
			auto texture_gray   = std::make_shared<Texture>(1, Color(0.8, 0.8, 0.8));
			auto texture_mettasphere = std::make_shared<Texture>(1, Color(0.7422, 0.71875, 0.859375));
			auto texture_metalsphere = std::make_shared<Texture>(1, Color(0.6289, 0.6602, 0.8125));
			auto texture_earth  = std::make_shared<Texture>(3, "resource\\earthmap.jpg");

			std::shared_ptr<OBJ_Loader> obj_loader = std::make_shared<OBJ_Loader>(&(this->primitives));
			obj_loader->read_obj("resource/Cornell_box/light.obj", 0,
								 material_light, texture_white,
								 2.0, 1.0, 2.0,
								 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
								 0.0, 0.0, 0.0,
								 false, false, false, false, false, false,
								 -300, 0.0, -300);

			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					obj_loader->read_obj("resource/boxes/unit_box.obj", 0,
										 material_matte, texture_boxes,
										 125.0, random_double(1, 101), 125.0,
										 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
										 0.0, 0.0, 0.0,
										 false, false, false, false, false, false,
										 -1000 + 125 * i, 0, -1000 + 125 * j);
				}
			}

			this->add_primitive(std::make_shared<Sphere>(Point3(220, 170, 145), 50, material_glass, texture_white));
			this->add_primitive(std::make_shared<Sphere>(Point3(0, 150, 145), 50, material_metal_2, texture_white));
			this->add_primitive(std::make_shared<Sphere>(Point3(400, 250, 400), 100, material_matte, texture_earth));
			this->add_primitive(std::make_shared<Sphere>(Point3(220, 330, 300), 80, material_matte, texture_mettasphere));
			this->add_primitive(std::make_shared<Sphere>(Point3(360, 160, 145), 70, material_metal, texture_metalsphere));

			break;
		}

		case 34:
		case 35:
		case 36: {
			auto material_matte = std::make_shared<Material>(0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			auto material_glass = std::make_shared<Material>(0.0, 0.0, 0.0, 0.8, 0.0, 1.5, 0.0, 0.0);
			auto material_metal = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

			auto texture_ground = std::make_shared<Texture>(1, Color(0.5, 0.5, 0.5));
			auto texture_earth = std::make_shared<Texture>(3, "resource\\earthmap.jpg");

			this->add_primitive(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, material_matte, texture_ground));

			auto texture_big_ball_1 = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
			this->add_primitive(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material_glass, texture_big_ball_1));

			auto texture_big_ball_2 = std::make_shared<Texture>(1, Color(0.4, 0.2, 0.1));
			this->add_primitive(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material_metal, texture_big_ball_2));

			auto texture_big_ball_3 = std::make_shared<Texture>(1, Color(0.7, 0.6, 0.5));
			this->add_primitive(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material_matte, texture_earth));

			for (int i = -10; i < 10; i++) {
				for (int j = -10; j < 10; j++) {
					auto  choose_material = random_double();
					Point3 center;

					int false_counter = 0; // 失败次数，避免球无法随机生成时出现死循环
					while (true) {
						if (false_counter > 1000) {
							center = Point3(0.0, -1000, 0.0);
							break;
						}

						center = Point3(i + 0.9 * random_double(), 0.2, j + 0.9 * random_double());
						center.set_y(sqrt(pow(1000.2, 2) - pow(center[0], 2) - pow(center[2], 2)) - 1000);

						if ((center - Point3(0.0, 1.0, 0.0)).length() < 1.3
							|| (center - Point3(4.0, 1.0, 0.0)).length() < 1.3
							|| (center - Point3(-4.0, 1.0, 0.0)).length() < 1.3) {
							false_counter++;
							continue;
						}

						bool inavailable = false;
						for (const auto sphere : this->primitives) {
							if ((center - sphere->get_reference_point()).length() < 0.5) {
								inavailable = true;
								break;
							}
						}
						if (inavailable) {
							false_counter++;
							continue;
						}

						break;
					}

					if (choose_material < 0.8) {
						auto texture = std::make_shared<Texture>(1, Color(random_double(), random_double(), random_double()));
						this->add_primitive(std::make_shared<Sphere>(center, 0.2, material_matte, texture));
					} else if (choose_material < 0.95) {
						auto material = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, random_double(0.0, 0.5));
						auto texture = std::make_shared<Texture>(1, Color(random_double(0.5, 1.0),
																		  random_double(0.5, 1.0),
																		  random_double(0.5, 1.0)));
						this->add_primitive(std::make_shared<Sphere>(center, 0.2, material_metal, texture));
					} else {
						auto texture = std::make_shared<Texture>(1, Color(1.0, 1.0, 1.0));
						this->add_primitive(std::make_shared<Sphere>(center, 0.2, material_glass, texture));
					}
				}
			}

			break;
		}
	}
}

Color Scene::ray_color(int renderer_type, Ray& ray) {
	switch (renderer_type) {
		case 10:
			return ambient_model(ray, 0);
		case 20:
			return Lambert_diffuse_model(ray, 0);
		case 30:
			return Phong_modle(ray, 0);
		case 40:
			return Blinn_Phong_model(ray, 0);
		case 51:
			return simplified_Whitted_ray_trace_by_weight(ray, 0);
		case 52:
			return simplified_Whitted_ray_trace_Monte_Carlo(ray, 0);
		default:
		case 61:
			return Whitted_ray_trace_by_weight(ray, 0);
		case 62:
			return Whitted_ray_trace_Monte_Carlo(ray, 0);
		case 71:
			return optimized_ray_trace_by_weight(ray, 0);
		case 72:
			return optimized_ray_trace_Monte_Carlo(ray, 0);
	}
}

void Scene::add_primitive(std::shared_ptr<Primitive> primitive) {
	primitives.push_back(primitive);
}

bool Scene::hit(const std::shared_ptr<BVH_Node> node, const Ray& ray, Hit_Record& record) const {
	switch (this->accelerator) {
		default:
		case 0:
			return hit_brute_force(ray, record);
		case 1:
		case 2:
		case 3:
			return hit_BVH(node, ray, record);
	}
}

bool Scene::hit_brute_force(const Ray& ray, Hit_Record& record) const {
	bool hit_anything = false;

	for (const auto& primitive : primitives) {
		if (primitive->hit(ray, record)) {
			hit_anything = true;
		}
	}

	return hit_anything;
}

bool Scene::hit_BVH(const std::shared_ptr<BVH_Node> node, const Ray& ray, Hit_Record& record) const {
	if (!node->hit(ray)) {
		return false;
	}

	if (node->is_leaf()) {
		// node is a leaf node in the BVH tree.
		// traverse all primitives in the node.
		bool hit_anything = false;
		for (int i = node->get_index(); i < node->get_index() + node->get_quantity(); i++) {
			if (primitives[i]->hit(ray, record)) {
				hit_anything = true;
			}
		}
		return hit_anything;
	} else {
		// node is not a leaf node in the BVH tree.
		bool left_hit  = hit_BVH(node->get_left(),  ray, record);
		bool right_hit = hit_BVH(node->get_right(), ray, record);
		return left_hit || right_hit;
	}
}

std::shared_ptr<BVH_Node> Scene::build_middle_BVH_tree(const int l, const int r) {
	if (l >= r) {
		return NULL;
	}

	std::shared_ptr<BVH_Node> node = std::make_shared<BVH_Node>(l, r - l);

	// Compute AABB.
	for (int i = l; i < r; i++) {
		node->get_AABB_reference().U(this->primitives[i]->get_AABB());
	}

	// Return this node directly if the quantity of primitives is smaller than 
	// the upper limit of quantity of primitives in a node.
	if (node->get_quantity() <= this->maximum_primitives_in_node) {
		return node;
	}

	// Sort this->primitives in the longest span axis.
	auto comparator = node->get_longest_span_axis() == 0 ? compare_x :
		              node->get_longest_span_axis() == 1 ? compare_y :
		                                                   compare_z;
	std::sort(this->primitives.begin() + l, this->primitives.begin() + r, comparator);

	// Find the middle_split.
	int axis = node->get_longest_span_axis();
	int middle_split = l + 1;
	double middle = (  this->primitives[l]    ->get_reference_point()[axis]
					 + this->primitives[r - 1]->get_reference_point()[axis]) / 2;
	
	for (int split = l + 1; split < r - 1; split++) {
		if (this->primitives[split]->get_reference_point()[axis] < middle) {
			middle_split = split;
		}
	}

	// Devide the longest span axis into two parts.
	node->set_left(build_SAH_BVH_tree(l, middle_split));
	node->set_right(build_SAH_BVH_tree(middle_split, r));

	return node;
}

std::shared_ptr<BVH_Node> Scene::build_equal_count_BVH_tree(const int l, const int r) {
	if (l >= r) {
		return NULL;
	}

	std::shared_ptr<BVH_Node> node = std::make_shared<BVH_Node>(l, r - l);

	// Compute AABB.
	for (int i = l; i < r; i++) {
		node->get_AABB_reference().U(this->primitives[i]->get_AABB());
	}

	// Return this node directly if the quantity of primitives is smaller than 
	// the upper limit of quantity of primitives in a node.
	if (node->get_quantity() <= this->maximum_primitives_in_node) {
		return node;
	}

	// Sort this->primitives in the longest span axis.
	auto comparator = node->get_longest_span_axis() == 0 ? compare_x :
		              node->get_longest_span_axis() == 1 ? compare_y :
		                                                   compare_z;
	std::sort(this->primitives.begin() + l, this->primitives.begin() + r, comparator);

	// Find the equal_count_split.
	int equal_count_split = (l + r) / 2;

	// Devide the longest span axis into two parts.
	node->set_left(build_equal_count_BVH_tree(l, equal_count_split));
	node->set_right(build_equal_count_BVH_tree(equal_count_split, r));

	return node;
}

std::shared_ptr<BVH_Node> Scene::build_SAH_BVH_tree(const int l, const int r) {
	if (l >= r) {
		return NULL;
	}

	std::shared_ptr<BVH_Node> node = std::make_shared<BVH_Node>(l, r - l);

	// Compute AABB.
	for (int i = l; i < r; i++) {
		node->get_AABB_reference().U(this->primitives[i]->get_AABB());
	}

	// Return this node directly if the quantity of primitives is smaller than 
	// the upper limit of quantity of primitives in a node.
	if (node->get_quantity() <= this->maximum_primitives_in_node) {
		return node;
	}

	// Sort this->primitives in the longest span axis.
	auto comparator = node->get_longest_span_axis() == 0 ? compare_x :
		              node->get_longest_span_axis() == 1 ? compare_y :
		                                                   compare_z;
	std::sort(this->primitives.begin() + l, this->primitives.begin() + r, comparator);

	// Find the SAH_split.
	int SAH_split = l + 1;
	double cost = INF;
	
	for (int split = l + 1; split < r - 1; split++) {
		AABB left, right;
		for (int i = l; i < split; i++) {
			left.U(this->primitives[i]->get_AABB());
		}
		for (int i = split; i < r; i++) {
			right.U(this->primitives[i]->get_AABB());
		}

		double split_cost = left.get_surface()  * (split - l)
			              + right.get_surface() * (r - split);
		if (split_cost < cost) {
			cost = split_cost;
			SAH_split = split;
		}
	}

	// Devide the longest span axis into two parts.
	node->set_left(build_SAH_BVH_tree(l, SAH_split));
	node->set_right(build_SAH_BVH_tree(SAH_split, r));

	return node;
}

double Scene::specular_proportion(const double ratio_of_refractive_indices, 
								  const double sin_in, const double cos_in, 
								  const double sin_out, const double cos_out) const {
	switch (this->config->get_RENDERER_TRANSMISSION_TRANSFORMATION_METHOD()) {
		case 1: {
			// Fresnel equation
			//       |eta_in*cos_in - eta_out*cos_out|2            |eta_in*cos_out - eta_out*cos_in|2 
			// R_s = |-------------------------------|       R_p = |-------------------------------| 
			//       |eta_in*cos_in + eta_out*cos_out|             |eta_in*cos_out - eta_out*cos_in|
			// F_R = 1/2 * (R_s + R_p)
			double R_s = pow((ratio_of_refractive_indices * cos_in - cos_out) / (ratio_of_refractive_indices * cos_in + cos_out), 2);
			double R_p = pow((ratio_of_refractive_indices * cos_out - cos_in) / (ratio_of_refractive_indices * cos_out + cos_in), 2);
			double F_R = 1.0 / 2 * (R_s + R_p);

			return F_R;
		}
		default:
		case 2:	{
			double r0  = pow((1 - ratio_of_refractive_indices) / (1 + ratio_of_refractive_indices), 2);
			double F_R = r0 + (1 - r0) * pow(1 - cos_in, 5);

			return F_R;
		}
	}
}

bool Scene::hit_lightsource(Ray& ray, Hit_Record& record) const {
	return !hit(this->BVH_tree, ray, record);
}

Color Scene::Lambert_diffuse_model(const Ray& ray, Hit_Record& record) const {
	// Lambert_diffuse_model
	// I = ka * Ia + kd * sum(n dot l_i).

	double ambient_reflection_coefficient = record.material->get_ambient_reflection_coefficient();
	double diffuse_reflection_coefficient = record.material->get_diffuse_reflection_coefficient();

	// ambient_intensity
	Color ambient_intensity(this->ambient_color);

	// diffuse_intensity
	Color diffuse_intensity(0.0, 0.0, 0.0);

	for (const auto& lightsource : this->lightsources) {
		Vector3    light = lightsource->get_position() - record.point;
		Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
		Hit_Record shadow_record;

		double lightsource_distance = light.length();
		shadow_record.distance = lightsource_distance + TOLERANCE;
		shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

		if (hit_lightsource(shadow_ray, shadow_record)) {
			light.unitization();
			Vector3 normal = record.normal.get_unit_vector();

			diffuse_intensity += lightsource->get_color()
				               * lightsource->get_intensity(lightsource_distance)
				               * fmax(0.0, dot_product(normal, light));
		} else {
			diffuse_intensity += lightsource->get_color()
				               * lightsource->get_intensity(lightsource_distance)
				               * shadow_record.light_pass_proportion;
		}
	}

	// I = ka * Ia + kd * Id.
	Color intensity = ambient_reflection_coefficient * ambient_intensity
		+ diffuse_reflection_coefficient * diffuse_intensity;

	Color albedo = record.texture->albedo(record.texture_point);
	return albedo * intensity;
}

Color Scene::ambient_model(const Ray& ray, int recursion_depth) const {
	// ambient_model
	// I = ka * Ia.

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient = record.material->get_ambient_reflection_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// I = ka * Ia.
		Color intensity = ambient_reflection_coefficient * ambient_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::Lambert_diffuse_model(const Ray& ray, int recursion_depth) const {
	// Lambert_diffuse_model
	// I = ka * Ia + kd * sum(n dot l_i).

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient = record.material->get_diffuse_reflection_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);

		if (diffuse_reflection_coefficient > TOLERANCE) {
			for (const auto& lightsource : this->lightsources) {
				Vector3    light = lightsource->get_position() - record.point;
				Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
				Hit_Record shadow_record;

				double lightsource_distance = light.length();
				shadow_record.distance = lightsource_distance + TOLERANCE;
				shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

				if (hit_lightsource(shadow_ray, shadow_record)) {
					light.unitization();
					Vector3 normal = record.normal.get_unit_vector();

					diffuse_intensity += lightsource->get_color()
						               * lightsource->get_intensity(lightsource_distance)
						               * fmax(0.0, dot_product(normal, light));
				}
			}
		}

		// I = ka * Ia + kd * Id.
		Color intensity = ambient_reflection_coefficient * ambient_intensity
			            + diffuse_reflection_coefficient * diffuse_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::Phong_modle(const Ray& ray, int recursion_depth) const {
	// Phong_model
	// I = ka * Ia + kd * sum(n dot l_i) + ks * sum(V dot R)^gloss
		
	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity and specular_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);
		Color specular_intensity(0.0, 0.0, 0.0);

		for (const auto& lightsource : this->lightsources) {
			Vector3    light = lightsource->get_position() - record.point;
			Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
			Hit_Record shadow_record;

			double lightsource_distance = light.length();
			shadow_record.distance = lightsource_distance + TOLERANCE;
			shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

			if (hit_lightsource(shadow_ray, shadow_record)) {
				light.unitization();
				Vector3 normal = record.normal.get_unit_vector();

				diffuse_intensity += lightsource->get_color()
					               * lightsource->get_intensity(lightsource_distance)
					               * fmax(0.0, dot_product(normal, light));

				// specular_intensity
				Vector3 V((config->get_CAMERA_POSITION() - record.point).get_unit_vector());
				Ray L(lightsource->get_position(), record.point - lightsource->get_position());
				Vector3 R(L.specular_reflect(record).get_direction().get_unit_vector());

				specular_intensity += lightsource->get_color()
					                * lightsource->get_intensity(lightsource_distance)
					                * pow(fmax(0.0, dot_product(V, R)), record.material->get_gloss());
			}
		}

		// I = ka * Ia + kd * Id + ks * Is.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::Blinn_Phong_model(const Ray& ray, int recursion_depth) const {
	// Blinn_Phong_model
	// I = ka * Ia + kd * sum(N dot l_i) + ks * sum(N dot H)^gloss
		
	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity and specular_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);
		Color specular_intensity(0.0, 0.0, 0.0);

		for (const auto& lightsource : this->lightsources) {
			Vector3    light = lightsource->get_position() - record.point;
			Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
			Hit_Record shadow_record;

			double lightsource_distance = light.length();
			shadow_record.distance = lightsource_distance + TOLERANCE;
			shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

			if (hit_lightsource(shadow_ray, shadow_record)) {
				light.unitization();
				Vector3 normal = record.normal.get_unit_vector();

				diffuse_intensity += lightsource->get_color()
					               * lightsource->get_intensity(lightsource_distance)
					               * fmax(0.0, dot_product(normal, light));

				// specular_intensity
				Vector3 V((config->get_CAMERA_POSITION() - record.point).get_unit_vector());
				Vector3 H((V + light).get_unit_vector());

				specular_intensity += lightsource->get_color()
						            * lightsource->get_intensity(lightsource_distance)
						            * pow(fmax(0.0, dot_product(normal, H)), record.material->get_gloss());
			}
		}

		// I = ka * Ia + kd * Id + ks * Is.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::simplified_Whitted_ray_trace_by_weight(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth)
		return Color(0.0, 0.0, 0.0);

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		if (record.material->get_diffuse_reflection_coefficient() > TOLERANCE) {
			// DIFFUSE
			return Lambert_diffuse_model(ray, record);
		} else if (record.material->get_refraction_coefficient() > TOLERANCE) {
			// REFLECT AND REFRACT
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				                                                     record.material->get_refractive_index(); // index_in / index_out
			double cos_in  = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in  = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R     = this->specular_proportion(ratio_of_refractive_indices,
												       sin_in, cos_in,
												       sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0) {                                                                              // Fresnel effect.
				return record.texture->albedo(record.texture_point)
					* (this->ambient_color
					   + record.material->get_refraction_coefficient() * (
						     F_R       * simplified_Whitted_ray_trace_by_weight(ray.specular_reflect(record), recursion_depth + 1)
						   + (1 - F_R) * simplified_Whitted_ray_trace_by_weight(ray.refract(record), recursion_depth + 1)));
			} else {
				// Total internal reflection.
				return record.texture->albedo(record.texture_point)
					* (this->ambient_color
					   + record.material->get_specular_reflection_coefficient()
					   * simplified_Whitted_ray_trace_by_weight(ray.specular_reflect(record), recursion_depth + 1));
			}
		} else {
			// REFLECT
			return record.texture->albedo(record.texture_point)
				* (this->ambient_color
				   + record.material->get_specular_reflection_coefficient()
				   * simplified_Whitted_ray_trace_by_weight(ray.specular_reflect(record), recursion_depth + 1));
		}
	} else {
		return this->ambient_color;
	}
}

Color Scene::simplified_Whitted_ray_trace_Monte_Carlo(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth)
		return Color(0.0, 0.0, 0.0);

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		if (record.material->get_diffuse_reflection_coefficient() > TOLERANCE) {
			// DIFFUSE
			return Lambert_diffuse_model(ray, record);
		} else if (record.material->get_refraction_coefficient() > TOLERANCE) {
			// REFLECT AND REFRACT
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				                                                     record.material->get_refractive_index(); // index_in / index_out
			double cos_in = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R = this->specular_proportion(ratio_of_refractive_indices,
												   sin_in, cos_in,
												   sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0                                                                                  // Fresnel effect.
				&& random_double() >= F_R) {
				return record.texture->albedo(record.texture_point)
					* (this->ambient_color
					   + record.material->get_refraction_coefficient()
					   * simplified_Whitted_ray_trace_Monte_Carlo(ray.refract(record), recursion_depth + 1));
			} else {
				// Total internal reflection.
				return record.texture->albedo(record.texture_point)
					* (this->ambient_color 
					   + record.material->get_refraction_coefficient()
					   * simplified_Whitted_ray_trace_Monte_Carlo(ray.specular_reflect(record), recursion_depth + 1));
			}
		} else {
			// REFLECT
			return record.texture->albedo(record.texture_point)
				* (this->ambient_color
				   + record.material->get_specular_reflection_coefficient()
				   * simplified_Whitted_ray_trace_Monte_Carlo(ray.specular_reflect(record), recursion_depth + 1));
		}
	} else {
		return this->ambient_color;
	}
}

Color Scene::Whitted_ray_trace_by_weight(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth) {
		return Color(0.0, 0.0, 0.0);
	}

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();
		double refraction_coefficient          = record.material->get_refraction_coefficient();
		double emission_coefficient            = record.material->get_emission_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);

		if (diffuse_reflection_coefficient > TOLERANCE) {
			for (const auto& lightsource : this->lightsources) {
				Vector3    light = lightsource->get_position() - record.point;
				Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
				Hit_Record shadow_record;

				double lightsource_distance = light.length();
				shadow_record.distance = lightsource_distance + TOLERANCE;
				shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

				if (hit_lightsource(shadow_ray, shadow_record)) {
					light.unitization();
					Vector3 normal = record.normal.get_unit_vector();

					diffuse_intensity += lightsource->get_color()
						               * lightsource->get_intensity(lightsource_distance)
						               * fmax(0.0, dot_product(normal, light));
				} else {
					diffuse_intensity += lightsource->get_color()
						               * lightsource->get_intensity(lightsource_distance)
						               * shadow_record.light_pass_proportion;
				}
			}
		}

		// refraction_intensity
		Color  refraction_intensity(0.0, 0.0, 0.0);

		if (refraction_coefficient > TOLERANCE) {
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				record.material->get_refractive_index(); // index_in / index_out
			double cos_in = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R = this->specular_proportion(ratio_of_refractive_indices,
												   sin_in, cos_in,
												   sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0) {                                                                                  // Fresnel effect.
				specular_reflection_coefficient += refraction_coefficient * F_R;
				refraction_coefficient *= (1.0 - F_R);

				refraction_intensity = Whitted_ray_trace_by_weight(ray.refract(record), recursion_depth + 1);
			} else {
				// Total internal reflection.
				specular_reflection_coefficient += refraction_coefficient;
				refraction_coefficient = 0.0;
			}
		}

		// specular_intensity
		Color specular_intensity(0.0, 0.0, 0.0);

		if (specular_reflection_coefficient > TOLERANCE) {
			specular_intensity = Whitted_ray_trace_by_weight(ray.specular_reflect(record), recursion_depth + 1);
		}

		// emitting_intensity
		Color emission_intensity(0.0, 0.0, 0.0);

		if (emission_coefficient > TOLERANCE) {
			emission_intensity = Color(1.0, 1.0, 1.0);
		}

		// Intensity_Whitted_style
		// I = ka * Ia + kd * sum(n dot l_i) + ks * Is + ke * Ir + ke * Ie.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity
			            + refraction_coefficient          * refraction_intensity
			            + emission_coefficient            * emission_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::Whitted_ray_trace_Monte_Carlo(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth) {
		return Color(0.0, 0.0, 0.0);
	}

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();
		double refraction_coefficient        = record.material->get_refraction_coefficient();
		double emission_coefficient            = record.material->get_emission_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);

		if (diffuse_reflection_coefficient > TOLERANCE) {
			for (const auto& lightsource : this->lightsources) {
				Vector3    light = lightsource->get_position() - record.point;
				Ray        shadow_ray(record.point, lightsource->get_position() - record.point);
				Hit_Record shadow_record;

				double lightsource_distance = light.length();
				shadow_record.distance = lightsource_distance + TOLERANCE;
				shadow_record.light_pass_proportion = Color(1.0, 1.0, 1.0);

				if (hit_lightsource(shadow_ray, shadow_record)) {
					light.unitization();
					Vector3 normal = record.normal.get_unit_vector();

					diffuse_intensity += lightsource->get_color()
						               * lightsource->get_intensity(lightsource_distance)
						               * fmax(0.0, dot_product(normal, light));
				} else {
					diffuse_intensity += lightsource->get_color()
						               * lightsource->get_intensity(lightsource_distance)
						               * shadow_record.light_pass_proportion;
				}
			}
		}

		// refraction_intensity
		Color  refraction_intensity(0.0, 0.0, 0.0);

		if (refraction_coefficient > TOLERANCE) {
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				record.material->get_refractive_index(); // index_in / index_out
			double cos_in = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R = this->specular_proportion(ratio_of_refractive_indices,
												   sin_in, cos_in,
												   sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0                                                                                  // Fresnel effect.
				&& random_double() >= F_R) {
				refraction_intensity = Whitted_ray_trace_Monte_Carlo(ray.refract(record), recursion_depth + 1);
			} else {
				// Total internal reflection.
				specular_reflection_coefficient += refraction_coefficient;
				refraction_coefficient = 0.0;
			}
		}

		// specular_intensity
		Color specular_intensity(0.0, 0.0, 0.0);

		if (specular_reflection_coefficient > TOLERANCE) {
			specular_intensity = Whitted_ray_trace_Monte_Carlo(ray.specular_reflect(record), recursion_depth + 1);
		}

		// emitting_intensity
		Color emission_intensity(0.0, 0.0, 0.0);

		if (emission_coefficient > TOLERANCE) {
			emission_intensity = Color(1.0, 1.0, 1.0);
		}

		// Intensity_Whitted_style
		// I = ka * Ia + kd * sum(n dot l_i) + ks * Is + ke * Ir + ke * Ie.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity
			            + refraction_coefficient        * refraction_intensity
			            + emission_coefficient            * emission_intensity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::optimized_ray_trace_by_weight(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth) {
		return Color(0.0, 0.0, 0.0);
	}

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();
		double refraction_coefficient        = record.material->get_refraction_coefficient();
		double emission_coefficient            = record.material->get_emission_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);

		if (diffuse_reflection_coefficient > TOLERANCE) {
			Vector3 random_vector_in_unit_sphere = random_vector_in_sphere(1.0);

			// Choose one diffuse ray generating method from Simple diffuse and true Lambertian reflection.
			// Method1: Simple diffuse.
			/*diffuse_ray = Ray(record.point,
							  (record.normal + random_vector_in_unit_sphere).near_zero() ?
							  record.normal : (record.normal + random_vector_in_unit_sphere));*/
			// Method2: True Lambertian reflection.
			Ray diffuse_ray(record.point,
							(record.normal + random_vector_in_unit_sphere.get_unit_vector()).near_zero() ?
							record.normal : (record.normal + random_vector_in_unit_sphere.get_unit_vector()));

			diffuse_intensity = optimized_ray_trace_by_weight(diffuse_ray, recursion_depth + 1);
		}

		// refraction_intensity
		Color  refraction_intensity(0.0, 0.0, 0.0);

		if (refraction_coefficient > TOLERANCE) {
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				record.material->get_refractive_index(); // index_in / index_out
			double cos_in = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R = this->specular_proportion(ratio_of_refractive_indices,
												   sin_in,  cos_in,
												   sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0) {                                                                                  // Fresnel effect.

				specular_reflection_coefficient += refraction_coefficient * F_R;
				refraction_coefficient *= (1.0 - F_R);

				refraction_intensity = optimized_ray_trace_by_weight(ray.refract(record), recursion_depth + 1);
			} else {
				// Total internal reflection.
				specular_reflection_coefficient += refraction_coefficient;
				refraction_coefficient = 0.0;
			}
		}

		// specular_intensity
		Color specular_intensity(0.0, 0.0, 0.0);

		if (specular_reflection_coefficient > TOLERANCE) {
			specular_intensity = optimized_ray_trace_by_weight(ray.specular_reflect(record), recursion_depth + 1);
		}

		// emission_intensity
		Color emission_intenity(0.0, 0.0, 0.0);

		if (emission_coefficient > TOLERANCE) {
			emission_intenity.set_rgb(1.0, 1.0, 1.0);
		}

		// path_trace_intensity
		// I = ka * Ia + kd * sum(n dot l_i) + ks * Is + ke * Ir + ke * Ie.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity
			            + refraction_coefficient        * refraction_intensity
			            + emission_coefficient            * emission_intenity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}

Color Scene::optimized_ray_trace_Monte_Carlo(const Ray& ray, int recursion_depth) const {
	if (recursion_depth > this->max_recursion_depth) {
		return Color(0.0, 0.0, 0.0);
	}

	Hit_Record record;
	record.distance = INF;

	if (this->hit(this->BVH_tree, ray, record)) {
		double ambient_reflection_coefficient  = record.material->get_ambient_reflection_coefficient();
		double diffuse_reflection_coefficient  = record.material->get_diffuse_reflection_coefficient();
		double specular_reflection_coefficient = record.material->get_specular_reflection_coefficient();
		double refraction_coefficient          = record.material->get_refraction_coefficient();
		double emission_coefficient            = record.material->get_emission_coefficient();

		// ambient_intensity
		Color ambient_intensity(this->ambient_color);

		// diffuse_intensity
		Color diffuse_intensity(0.0, 0.0, 0.0);

		if (diffuse_reflection_coefficient > TOLERANCE) {
			Vector3 random_vector_in_unit_sphere = random_vector_in_sphere(1.0);

			// Choose one diffuse ray generating method from Simple diffuse and true Lambertian reflection.
			// Method1: Simple diffuse.
			/*diffuse_ray = Ray(record.point,
							  (record.normal + random_vector_in_unit_sphere).near_zero() ?
							  record.normal : (record.normal + random_vector_in_unit_sphere));*/

			// Method2: True Lambertian reflection.
			Ray diffuse_ray(record.point,
							(record.normal + random_vector_in_unit_sphere.get_unit_vector()).near_zero() ?
							record.normal : (record.normal + random_vector_in_unit_sphere.get_unit_vector()));

			diffuse_intensity = optimized_ray_trace_Monte_Carlo(diffuse_ray, recursion_depth + 1);
		}

		// refraction_intensity
		Color refraction_intensity(0.0, 0.0, 0.0);

		if (refraction_coefficient > TOLERANCE) {
			double ratio_of_refractive_indices = record.front_face ? (1.0 / record.material->get_refractive_index()) :
				record.material->get_refractive_index(); // index_in / index_out
			double cos_in = fmin(dot_product(-ray.get_direction().get_unit_vector(), record.normal), 1.0);
			double sin_in = sqrt(1.0 - pow(cos_in, 2));
			double sin_out = ratio_of_refractive_indices * sin_in;
			double cos_out = sqrt(1.0 - pow(sin_out, 2));
			double F_R = this->specular_proportion(ratio_of_refractive_indices,
												   sin_in, cos_in,
												   sin_out, cos_out);

			// Judge whether to produce refracted ray, according to the Fresnal effect.
			if (sin_out < 1.0                                                                                  // Fresnel effect.
				&& random_double() > F_R) {
				refraction_intensity = optimized_ray_trace_Monte_Carlo(ray.refract(record), recursion_depth + 1);
			} else {
				// Total internal reflection.
				specular_reflection_coefficient += refraction_coefficient;
				refraction_coefficient = 0.0;
			}
		}

		// specular_intensity
		Color specular_intensity(0.0, 0.0, 0.0);

		if (specular_reflection_coefficient > TOLERANCE) {
			specular_intensity = optimized_ray_trace_Monte_Carlo(ray.specular_reflect(record), recursion_depth + 1);
		}

		// emission_intensity
		Color emission_intenity(0.0, 0.0, 0.0);

		if (emission_coefficient > TOLERANCE) {
			emission_intenity.set_rgb(1.0, 1.0, 1.0);
		}

		// path_trace_intensity
		// I = ka * Ia + kd * sum(n dot l_i) + ks * Is + ke * Ir + ke * Ie.
		Color intensity = ambient_reflection_coefficient  * ambient_intensity
			            + diffuse_reflection_coefficient  * diffuse_intensity
			            + specular_reflection_coefficient * specular_intensity
			            + refraction_coefficient        * refraction_intensity
			            + emission_coefficient            * emission_intenity;

		Color albedo = record.texture->albedo(record.texture_point);
		return albedo * intensity;
	} else {
		return this->ambient_color;
	}
}