#include "Camera.h"
#include "RayTracer.h"

Ray Camera::getRay(glm::vec2 _pixel)
{
	return Ray(m_position, vec3(1.0f));
}