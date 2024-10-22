#include <ctime>
#include <omp.h>
#include <string>

#include "../include/Canvas.h"
#include "../include/Camera.h"
#include "../include/constants.h"
#include "../include/Renderer.h"
#include "../include/Scene.h"

int main(int argc, char* argv[]) {
	// Build framwork(Config, Scene, Camera, Canvas and Renderer).
	state_output(1, 0.0);
	auto framework_start_time = std::clock();
	std::shared_ptr<Config>   config   = std::make_shared<Config>(argv[1]);
	std::shared_ptr<Scene>    scene    = std::make_shared<Scene>(config);
	std::shared_ptr<Camera>   camera   = std::make_shared<Camera>(config);
	std::shared_ptr<Canvas>   canvas   = std::make_shared<Canvas>(config);
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(config, scene, camera);
	state_output(2, double(std::clock() - framework_start_time));

	// Render.
	state_output(3, 0.0);
	double total_render_time = 0.0;
	
	for (int sample = 0; sample < config->get_IMAGE_SAMPLES_PER_PIXEL(); sample++) {
		state_output(4, sample, config->get_IMAGE_SAMPLES_PER_PIXEL());
		auto render_sample_start_time = std::clock();

		omp_set_num_threads(config->get_MULTITHREADS_NUMBER_OF_THREADS());
	    #pragma omp parallel for
		for (int row = 0; row < config->get_IMAGE_HEIGHT(); row++) {
			for (int column = 0; column < config->get_IMAGE_WIDTH(); column++) {
				Color color = renderer->render(row, column);
				canvas->add_pixel_color(row, column, color);
			}
			if (config->get_MULTITHREADS_NUMBER_OF_THREADS() == 1) {
				state_output(5, double(row) / config->get_IMAGE_HEIGHT());
			}
		}

		double sample_render_time = double(std::clock() - render_sample_start_time);
		total_render_time += sample_render_time;
		canvas->generate_image(false, sample + 1, sample_render_time / CLOCKS_PER_SEC);
		state_output(6, sample_render_time);
	}
	state_output(7, total_render_time);

	// Generate image.
	canvas->generate_image(true, config->get_IMAGE_SAMPLES_PER_PIXEL(), total_render_time / CLOCKS_PER_SEC);
	state_output(8, 0.0);

	return 0;
}