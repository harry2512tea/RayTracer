#include "Camera.h"
#include "RayTracer.h"
#include <iostream>

vec3 Camera::m_position = vec3(0.0f);

Camera::Camera()
{
	view = translate(view, -m_position);
	projection = perspective(radians(70.0f), (float)640/(float)480, 0.1f, 100.0f);
}

Ray Camera::getRay(glm::vec2 _pixel)
{
	//float screen_distance = 0.5f / tan(glm::radians(70.0f) / 2.0f);

	if (_pixel.x == 320 &&_pixel.y == 240)
	{
		std::cout << "now" << std::endl;
	}

	vec3 rayStart = vec3((_pixel.x - 320) / 320, (_pixel.y - 240) / 240, -1);
	vec3 rayEnd = vec3((_pixel.x - 320) /320, (_pixel.y - 240) / 240, 1);

	vec4 tempRayStart = vec4(rayStart, 1.0f) * inverse(projection);
	vec4 tempRayEnd = vec4(rayEnd, 1.0f) * inverse(projection);

	rayStart = tempRayStart;
	rayEnd = tempRayEnd;

	rayStart /= tempRayStart.w;
	rayEnd /= tempRayEnd.w;

	rayStart = vec4(rayStart, 1.0f) * inverse(view);
	rayEnd = vec4(rayEnd, 1.0f) * inverse(view);

	Ray ray;
	
	ray.m_direction = normalize(rayEnd - rayStart);
	ray.m_origin = rayStart;

	return ray;
}