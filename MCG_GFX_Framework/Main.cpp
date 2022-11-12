
#include <cmath>

#include "MCG_GFX_Lib.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Object.h"



int main( int argc, char *argv[] )
{
	// Variable for storing window dimensions
	glm::ivec2 windowSize( 640, 480 );

	// Call MCG::Init to initialise and create your window
	// Tell it what size you want the window to be
	if( !MCG::Init( windowSize ) )
	{
		// We must check if something went wrong
		// (this is very unlikely)
		return -1;
	}

	// Sets every pixel to the same colour
	// parameters are RGB, numbers are from 0 to 1
	MCG::SetBackground( glm::vec3(0,0,0) );

	// Preparing a position to draw a pixel
	glm::ivec2 pixelPosition = windowSize / 2;

	// Preparing a colour to draw
	// Colours are RGB, each value ranges between 0 and 1
	glm::vec3 pixelColour( 1, 0, 0 );


	// Draws a single pixel at the specified coordinates in the specified colour!
	//MCG::DrawPixel( pixelPosition, pixelColour );

	// Do any other DrawPixel calls here
	// ...

	// Displays drawing to screen and holds until user closes window
	// You must call this after all your drawing calls
	// Program will exit after this line
	//return MCG::ShowAndHold();





	// Advanced access - comment out the above DrawPixel and MCG::ShowAndHold lines, then uncomment the following:
	
	
	// Variable to keep track of time
	float timer = 0.0f;

	Camera cam = Camera();
	Tracer tracer = Tracer();


	// This is our game loop
	// It will run until the user presses 'escape' or closes the window
	while( MCG::ProcessFrame() )
	{
		MCG::SetBackground( glm::vec3(0,0,0) );

		for (int x = 0; x < windowSize.x; x++)
		{
			for (int y = 0; y < windowSize.y; y++)
			{
				pixelColour = tracer.getColour(cam.getRay(glm::vec2(x, y)));

				MCG::DrawPixel(glm::vec2(x, y), pixelColour);
			}
		}
	}

	return 0;
	

}

rayCastHit RaySphereIntersect(Ray _ray, vec3 _pos, float _r)
{
	rayCastHit hit;

	vec3 a = _ray.getOrigin();
	vec3 n = _ray.getDirection();
	vec3 P = _pos;
	float d = (P - a - getPointOnLine(_ray, _pos)).length();
	float x = sqrt((_r * _r) - (d * d));

	vec3 point = a + (dot((P - a), n) - x) * n;

	if (point.length() <= _r)
	{
		hit.hit = true;
		hit.point = point;
		hit.distance = (point - _ray.getOrigin()).length();
		hit.normal = getNormal(_pos, point);
	}
	else
	{
		hit.hit = false;
	}

	return hit;
}

vec3 getPointOnLine(Ray _ray, glm::vec3 _point)
{
	vec3 a = _ray.getOrigin();
	vec3 n = _ray.getDirection();
	vec3 P = _point;
	return (dot((P - a), n)) * n;
}

vec3 getNormal(glm::vec3 _SphereCenter, vec3 _intersectPoint)
{
	return normalize(_intersectPoint - _SphereCenter);
}
