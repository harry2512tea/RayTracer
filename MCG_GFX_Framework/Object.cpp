#include "Object.h"
#include "RayTracer.h"
#include "Camera.h"

vec3 Sphere::shadePixel(Ray _ray, glm::vec3 _intersect, vec3 _LightPos)
{
	vec3 N = getNormal(_intersect);

	vec3 LightDir = normalize(_LightPos - _intersect);

	float diff = max(dot(N, LightDir), 0.0f);
	vec3 diffuse = vec3(1.0f) * diff;

	float specularStrength = 1.0f;
	vec3 viewDir = normalize(Camera::getPos() - _intersect);
	vec3 reflectDir = reflect(-LightDir, N);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 100);
	vec3 specular = specularStrength * spec * vec3(1.0f);

	vec3 L = max(diffuse, vec3(0.1f)) + specular;

	return L * colour;
}

vec3 Sphere::getNormal(vec3 _intersect)
{
	return normalize(_intersect - m_position);
}



