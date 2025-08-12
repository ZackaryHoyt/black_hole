#include "engine.h"

#include <iostream>
#include <cstdlib>

Engine::Engine()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(WIDTH, HEIGHT, "Black Hole Simulation", nullptr, nullptr);
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

	glViewport(0, 0, WIDTH, HEIGHT);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double left   = -width + offsetX;
	double right  =  width + offsetX;
	double bottom = -height + offsetY;
	double top    =  height + offsetY;
	glOrtho(left, right, bottom, top, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
