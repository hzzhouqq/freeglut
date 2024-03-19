#include "glut_helper.hpp"

#include <math.h>
#include <GLFW/glfw3.h>

void glx_perspective(float vfov, float aspect, float znear, float zfar)
{
	const float m_pi = 3.14159265358979323846f;
	float vfov_rad = (float)m_pi * vfov / 180.f;
	float x = znear * (float)tan(vfov_rad / 2.f);
	glFrustum(-aspect * x, aspect * x, -x, x, znear, zfar);
}
