#include "Object.h"
#include "RayTracer.h"
#include "Camera.h"


Sphere::Sphere(int _ID, vec3 Pos, vec3 _albedo)
{
	m_position = Pos;
	albedo = _albedo;
	ID = _ID;
}

vec3 Sphere::shadePixel(Ray _ray, glm::vec3 _intersect, vec3 _LightPos, int depth, std::list<Shared<Sphere>>* Objs)
{
	vec3 N = getNormal(_intersect);

	vec3 LightDir = normalize(_LightPos - _intersect);

	float diff = max(dot(N, LightDir), 0.0f);
	/*vec3 diffuse = vec3(1.0f) * diff;

	float specularStrength = 1.0f;
	vec3 viewDir = normalize(Camera::getPos() - _intersect);
	vec3 reflectDir = reflect(-LightDir, N);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);
	vec3 specular = specularStrength * spec * vec3(1.0f);

	vec3 L = max(diffuse, vec3(0.1f)) + specular;

	return L * albedo;*/

	vec3 spec = SpecularBRDF(_intersect, _LightPos);
	vec3 diffuse = DiffuseBRDF(_intersect, _LightPos);

	vec3 BRDF =  diffuse + spec;

	vec3 I;
	if (depth < maxDepth)
	{
		float maxScatter = 90.0f;
		
		int maxRayNumbers = 20;
		int rayNumbers = (int)20 * 1 - Roughness;

		I = BRDF * vec3(1.0f) * dot(N, normalize(_LightPos - _intersect));

		for (int i = 0; i < rayNumbers; i++)
		{
			quat QuatAroundX = angleAxis(radians(linearRand(-90.0f, 90.0f) * 1 - Roughness), vec3(1.0, 0.0, 0.0));
			quat QuatAroundY = angleAxis(radians(linearRand(-90.0f, 90.0f) * 1 - Roughness), vec3(0.0, 1.0, 0.0));
			quat QuatAroundZ = angleAxis(radians(linearRand(-90.0f, 90.0f) * 1 - Roughness), vec3(0.0, 0.0, 1.0));
			quat finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
			vec3 dir = N * finalOrientation;;
			Ray ray = Ray(_intersect, dir);

			//vec3 secondaryLight = Tracer::getColour(ray, Objs, depth + 1, ID) * dot(N, ray.getDirection());
			//vec3 primaryLight = vec3(1.0f) * dot(N, normalize(_LightPos - _intersect));


			I += BRDF * (Tracer::getColour(ray, Objs, depth + 1, ID)) * dot(N, ray.getDirection());
		}

		
		//I += BRDF * (Tracer::getColour(ray, Objs, depth + 1, ID)) * (1.0f - Roughness) * dot(N, normalize(_LightPos - _intersect));
	}
	else
	{
		I = BRDF * vec3(1.0f) * dot(N, normalize(_LightPos - _intersect));
		//I = vec3(1.0f);
	}


	return I;
}

vec3 Sphere::getNormal(vec3 _intersect)
{
	return normalize(_intersect - m_position);
}

vec3 Sphere::DiffuseBRDF(vec3 _intersect, vec3 _lightPos)
{
	vec3 N = getNormal(_intersect);
	vec3 LightDir = normalize(_lightPos - _intersect);

	vec3 F = F0 + (1.0f - F0) * pow((1 - cos(dot(N, LightDir))), 5);
	vec3 diffuseBRDF = (1.0f - F) * (albedo / pi<float>());

	return diffuseBRDF;
}

vec3 Sphere::SpecularBRDF(vec3 _intersect, vec3 _lightPos)
{
	vec3 N = getNormal(_intersect);
	vec3 L = normalize(_lightPos - _intersect);
	vec3 V = normalize(Camera::getPos() - m_position);

	vec3 H = normalize(V + L);

	vec3 F = fresnelSchlick(max(dot(N, L), 0.0f));

	vec3 specularBRDF = (F * DistributionGGX(N, H) * GeomSmith(N, V, L)) / (4 * max(dot(N, L), 0.0f) * max(dot(N, V), 0.0f));

	return specularBRDF;
}

float Sphere::DistributionGGX(vec3 N, vec3 H)
{
	float a = Roughness * Roughness;
	float a2 = a * a;

	float NdotH = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom *= denom;

	float NDF = a2 / denom;

	return NDF;
}

float Sphere::GeomSchlickGGX(float NdotV)
{
	float r = (Roughness + 1.0f);
	float k = (r * r) / 8.0f;

	return NdotV / (NdotV * (1.0f - k) + k);
}

float Sphere::GeomSmith(vec3 N, vec3 V, vec3 L)
{
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2 = GeomSchlickGGX(NdotV);
	float ggx1 = GeomSchlickGGX(NdotL);

	return ggx1 * ggx2;
}

vec3 Sphere::fresnelSchlick(float cosTheta)
{
	return F0 + (1.0f - F0) * (float)pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
