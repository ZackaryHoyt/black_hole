#include "engine2d.h"

#include <iostream>
#include <cstdlib>

Engine::Engine(const int window_width, const int window_height, const double scene_scale)
	: window_width(window_width), window_height(window_height), scene_scale(scene_scale)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(window_width, window_height, "Black Hole Simulation", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW after a valid context is current
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glViewport(0, 0, window_width, window_height);
}

Engine::~Engine()
{
	if (window)
	{
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}

void Engine::run()
{
	const double scene_rw = get_scene_width() / 2, scene_rh = get_scene_height() / 2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double x0 = camera_cx - scene_rw, x1 = camera_cx + scene_rw;
	double y0 = camera_cy - scene_rh, y1 = camera_cy + scene_rh;
	glOrtho(x0, x1, y0, y1, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

double Engine::get_scene_width() const { return window_width * scene_scale; }
double Engine::get_scene_height() const { return window_height * scene_scale; }
