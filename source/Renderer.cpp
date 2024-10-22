#include "../include/Renderer.h"

Renderer::Renderer(std::shared_ptr<Config> config, 
				   std::shared_ptr<Scene>  scene, 
				   std::shared_ptr<Camera> camera) {
	this->config = config;
	this->option = config->get_RENDERER_OPTION();

	this->scene  = scene;
	this->camera = camera;
}

Renderer::~Renderer() {
}

Color Renderer::render(const int row, const int column) {
	Ray ray = camera->get_ray(row, column);
	if (row == 364 && column == 199) {
		int i = 1;
		i += 1;
	}
	Color color = scene->ray_color(this->option, ray);

	return color;
}
