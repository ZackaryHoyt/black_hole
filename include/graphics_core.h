#pragma once

#define GLFW_INCLUDE_NONE
#ifdef _WIN32
// On Windows, include windows.h before GLEW to ensure wgl types are available
#include <windows.h>
#endif

// Make sure GLEW is included before GLFW and that GLFW doesn't include GL headers itself
#include <GL/glew.h>
#include <GLFW/glfw3.h>
