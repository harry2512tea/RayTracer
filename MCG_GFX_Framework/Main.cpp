
#include <cmath>
#include <memory>
#include <list>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <SDL/SDL.h>

#include "MCG_GFX_Lib.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Object.h"
#include "Plane.h"


#define Shared std::shared_ptr
#define Weak std::weak_ptr

using namespace glm;

std::mutex mutex;

struct forValues
{
	int xStart, yStart, xEnd, yEnd;
	Camera* cam;
	Tracer* tracer;
	ivec2 windowSize;
};

void threadFunc(forValues vals, std::list<Shared<Sphere>>& objs, std::list<Shared<Plane>>& planes, std::vector<unsigned char>& _pixels)
{
	for (int x = vals.xStart; x < vals.xEnd; x++)
	{

		for (int y = vals.yStart; y < vals.yEnd; y++)
		{
			rayCastHit hit;
			vec3 pixelColour;
			pixelColour = vals.tracer->getColour(vals.cam->getRay(glm::vec2(x, y)), &objs, &planes, 0, 0, hit);

			glm::clamp(pixelColour.x, 0.0f, 1.0f);
			glm::clamp(pixelColour.y, 0.0f, 1.0f);
			glm::clamp(pixelColour.z, 0.0f, 1.0f);

			mutex.lock();
			const unsigned int offset = (vals.windowSize.x * y * 4) + x * 4;
			_pixels[offset + 0] = (pixelColour.z * 255);
			_pixels[offset + 1] = (pixelColour.y * 255);
			_pixels[offset + 2] = (pixelColour.x * 255);
			_pixels[offset + 3] = SDL_ALPHA_OPAQUE;
			MCG::DrawPixel(glm::ivec2(x, y), pixelColour);
			mutex.unlock();
		}

	}
}

int main( int argc, char *argv[] )
{
	
	// Variable for storing window dimensions
	glm::ivec2 windowSize( 640, 480 );
	//glm::ivec2 windowSize(1920, 1080);

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

	int pass = 0;

	int xblocks = 16;
	int yblocks = 12;

	std::list<Shared<Sphere>> m_objects;
	std::list<Shared<Plane>> m_planes;
	m_objects.push_back(std::make_shared<Sphere>(1, vec3(1.0f, -0.0f, -11.0f), vec3(0.0f, 0.0f, 1.0f)));
	m_objects.push_back(std::make_shared<Sphere>(2, vec3(-0.5f, -1.0f, -12.0f), vec3(1.0f, 1.0f, 1.0f)));



	m_objects.push_back(std::make_shared<Sphere>(3, vec3(0.0f, 1.0f, -10.0f), vec3(1.0f, 1.0f, 1.0f)));
	m_objects.back()->setRadius(5.0f);
	m_objects.back()->setRoughness(1.0f);
	//m_objects.push_back(std::make_shared<Sphere>(4, vec3(-1.5f, 1.0f, -15.0f), vec3(0.0f, 0.0f, 1.0f)));
	//m_objects.push_back(std::make_shared<Sphere>(3, vec3(-1.5f, 1.5f, -17.0f), vec3(1.0f, 0.0f, 0.0f)));
	//m_objects.push_back(std::make_shared<Sphere>());


	//m_planes.push_back(std::make_shared<Plane>());

	// This is our game loop
	// It will run until the user presses 'escape' or closes the window
	
	std::vector<std::thread> threads;
	SDL_Texture* texture = SDL_CreateTexture(MCG::getRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowSize.x, windowSize.y);
	std::vector<unsigned char> pixels(windowSize.x * windowSize.y * 4, 0);


	int ticks;
	while( MCG::ProcessFrame() )
	{
		threads.clear();
		MCG::SetBackground( glm::vec3(0,0,0) );

		std::cout << "Pass No: " << pass << std::endl;

		
		/*for (int x = 0; x < windowSize.x; x++)
		{
			
			for (int y = 0; y < windowSize.y; y++)
			{
				rayCastHit hit;
				pixelColour = tracer.getColour(cam.getRay(glm::vec2(x, y)), &m_objects, 0, 0, hit);

				MCG::DrawPixel(glm::ivec2(x, y), pixelColour);
			}
			
		}*/
		int xBlockSize = windowSize.x / xblocks;
		int yBlockSize = windowSize.y / yblocks;

		for (int x = 0; x < xblocks; x++)
		{

			for (int y = 0; y < yblocks; y++)
			{
				//rayCastHit hit;
				//pixelColour = tracer.getColour(cam.getRay(glm::vec2(x, y)), &m_objects, 0, 0, hit);

				///MCG::DrawPixel(glm::ivec2(x, y), pixelColour);

				forValues vals;
				vals.xStart = xBlockSize * x;
				vals.xEnd = vals.xStart + xBlockSize;
				vals.yStart = yBlockSize * y;
				vals.yEnd = vals.yStart + yBlockSize;
				vals.tracer = &tracer;
				vals.cam = &cam;

				threads.push_back(std::thread(threadFunc, vals, std::ref(m_objects), std::ref(m_planes), std::ref(pixels)));
			}

		}

		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}


		unsigned char* lockedPixels = nullptr;
		int pitch = 0;
		//SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&lockedPixels), &pitch);
		//std::copy_n(pixels.data(), pixels.size(), lockedPixels);
		//SDL_UnlockTexture(texture);

		//SDL_UpdateTexture(texture, NULL, pixels.data(), windowSize.x * 4);
		//SDL_RenderCopy(MCG::getRenderer(), texture, nullptr, nullptr);
		//SDL_RenderPresent(MCG::getRenderer());

 		pass++;
	}

	return 0;
	

}




