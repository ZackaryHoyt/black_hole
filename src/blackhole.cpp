#include "blackhole.h"
#include "constants.h"

#include <GL/gl.h>

BlackHole::BlackHole(glm::vec3 p, float m)
	: p(p), m(m)
{
	r_s = 2.0 * G * m / (C*C);
}

void BlackHole::draw()
{
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for the black hole.
	glVertex2f(0.0f, 0.0f); // Center
	for(int i = 0; i <= GLM_CIRCLE_SMOOTHNESS; i++)
	{
		float angle = 2.0f * PI * i / GLM_CIRCLE_SMOOTHNESS;
		float x = r_s * cos(angle);
		float y = r_s * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
}
