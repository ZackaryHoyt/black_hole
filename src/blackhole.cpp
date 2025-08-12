#include "blackhole.h"
#include "constants.h"

BlackHole::BlackHole(glm::vec3 p, float m)
	: p(p), m(m)
{
	r_s = 2.0 * G * m / (C*C);
}
