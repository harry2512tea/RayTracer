#include "RayTracer.h"

Ray::Ray(vec3 _origin, vec3 _direction)
{
	m_direction = _direction;
	m_origin = _origin;
}
glm::vec3 Tracer::getColour(Ray _ray)
{
	return glm::vec3(1.0f);
}