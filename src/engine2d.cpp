#include "engine2d.h"
#include "colors.h"
#include "constants.h"

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

void Engine::draw(const SchwarzschildUniverse &universe) const
{
	// Render Blackhole
	const BlackHole& blackhole = universe.get_blackhole();

	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(BLACK_HOLE_COLOR);
	glVertex2f(blackhole.p.x, blackhole.p.y); // Center
	for(int i = 0; i <= GLM_CIRCLE_SMOOTHNESS; i++)
	{
		double angle = 2.0f * PI * i / GLM_CIRCLE_SMOOTHNESS;
		double x = blackhole.r_s * cos(angle);
		double y = blackhole.r_s * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
	
	// Render Ray Points
	glPointSize(2.0f);
	glColor3fv(RAY_COLOR);
	glBegin(GL_POINTS);
	for (const auto& ray : universe.get_rays())
	{
		glVertex2f(ray.x, ray.y);
	}
	glEnd();

	// Render Ray Trails
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);

	// draw each trail with fading alpha
	for (const auto& ray : universe.get_rays())
	{
		int n_points = ray.trail.size();

		if (n_points < 2)
		{
			continue;
		}

		double alpha_multiplier = 1.0 / (2 * n_points);

		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < n_points; ++i)
		{
			glColor4f(RAY_COLOR[0], RAY_COLOR[1], RAY_COLOR[2], i * alpha_multiplier);
			glVertex2f(ray.trail[i].x, ray.trail[i].y);
		}
		glEnd();
	}

	glDisable(GL_BLEND);
}
