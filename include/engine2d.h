#pragma once

#include "graphics_core.h"

struct Engine
{
	GLFWwindow* window;
	int window_width = 800, window_height = 600;

	double scene_scale;

	double camera_cx = 0, camera_cy = 0, camera_zoom = 1.0f;

	bool middleMousePressed = false;
	double lastMouseX = 0.0, lastMouseY = 0.0;

	Engine(const int window_width=800, const int window_height=600, const double scene_scale=250e6);

	~Engine();

	void run();

	double get_scene_width() const;
	double get_scene_height() const;
};
