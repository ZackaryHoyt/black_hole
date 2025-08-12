#include "blackhole.h"
#include "constants.h"
#include "colors.h"

BlackHole::BlackHole(glm::vec3 p, float m)
	: p(p), m(m)
{
	r_s = 2.0 * G * m / (C*C);
}

void BlackHole::draw()
{
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(BLACK_HOLE_COLOR);
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
