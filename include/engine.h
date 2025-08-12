#pragma once

// Forward declaration to avoid pulling in GL headers from headers
struct GLFWwindow;

struct Engine
{
	GLFWwindow* window;
	int WIDTH = 800;
	int HEIGHT = 600;
	float width = 100000000000.0f; // Width of the viewport in meters
	float height = 75000000000.0f; // Height of the viewport in meters

	// Navigation state
	float offsetX = 0.0f, offsetY = 0.0f;
	float zoom = 1.0f;
	bool middleMousePressed = false;
	double lastMouseX = 0.0, lastMouseY = 0.0;

	Engine();
	~Engine();
	void run();
};
