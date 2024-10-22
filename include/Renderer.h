#pragma once

#include "Color.h"
#include "Scene.h"
#include "Camera.h"
#include "Canvas.h"

class Renderer {
public:
	// Constructors and Destructor
	// ---------------------------
	Renderer(std::shared_ptr<Config> config,
			 std::shared_ptr<Scene>  scene,
			 std::shared_ptr<Camera> camera);
	~Renderer();

	// Other functions
	// ---------------
	Color render(const int row, const int column);

private:
	std::shared_ptr<Config> config;
	std::shared_ptr<Scene>  scene;
	std::shared_ptr<Camera> camera;

	int option;
};