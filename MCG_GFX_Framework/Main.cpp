#include <cmath>
#include "MCG_GFX_Lib.h"
#include <GLM/gtx/string_cast.hpp>
#include <iostream>
#include "Shapes.h"
#include <vector>
#include <cstdlib>

void drawScreen(glm::ivec2 _windowSize, RayTracer _tracer, Camera _camera);

int main( int argc, char *argv[] )
{
	
	glm::ivec2 windowSize(640, 480);
	glm::ivec2 pixelPosition = windowSize / 2;
	if( !MCG::Init( windowSize ) )
	{
		return -1;
	}
	RedPlastic plastic;
	YellowPlastic plastic2; //create instances of the materials
	RayTracer tracer;
	sphere ball(40.0f, glm::vec4(150.0f, 100.0f, -600.0f, 1.0f), glm::vec3(1.0f, 0, 0), &plastic2);
	sphere ball2(10.0f, glm::vec4(50.0f, 100.0f, -600.0f, 1.0f), glm::vec3(1.0f, 0, 0), &plastic);
	sphere ball3(10.0f, glm::vec4(80.0f, 70.0f, -600.0f, 1.0f), glm::vec3(1.0f, 0, 0), &plastic);
	sphere ball4(10.0f, glm::vec4(230.0f, 70.0f, -600.0f, 1.0f), glm::vec3(1.0f, 0, 0), &plastic); //creating all the spheres used for the animation
	Camera mainCamera;
	glm::mat4 rotation, translation1, translation2, rotation2, rotation3; //setting up matrices used for transformations
	glm::vec3 move(ball.getCenter().x, ball.getCenter().y, ball.getCenter().z);
	translation1 = glm::translate(translation1, -move);
	rotation = glm::rotate(rotation, glm::radians(2.0f), glm::vec3(0, 1, 0));
	rotation2 = glm::rotate(rotation2, glm::radians(2.0f), glm::normalize(glm::vec3(-1, 1, 0)));
	rotation3 = glm::rotate(rotation3, glm::radians(2.0f), glm::normalize(glm::vec3(1, 1, 0)));
	translation2 = glm::translate(translation2, move); //initialises all the transformations
	tracer.addShape(&ball);
	tracer.addShape(&ball2);
	tracer.addShape(&ball3);
	tracer.addShape(&ball4); //adds the spheres to the ray tracer's vector
	while (true)
	{
		drawScreen(windowSize, tracer, mainCamera); //display the frame
		ball2.transform(translation1, glm::vec4(0, 0, 0, 0));
		ball2.transform(rotation, glm::vec4(0, 0, 0, 0));
		ball2.transform(translation2, glm::vec4(0, 0, 0, 0));
		ball3.transform(translation1, glm::vec4(0, 0, 0, 0));
		ball3.transform(rotation2, glm::vec4(0, 0, 0, 0));
		ball3.transform(translation2, glm::vec4(0, 0, 0, 0));
		ball4.transform(translation1, glm::vec4(0, 0, 0, 0));
		ball4.transform(rotation3, glm::vec4(0, 0, 0, 0));
		ball4.transform(translation2, glm::vec4(0, 0, 0, 0)); //each frame perform the transformations on each ball
		MCG::ProcessFrame();
	}
	MCG::ShowAndHold();
	return 0;
}

void drawScreen(glm::ivec2 _windowSize, RayTracer _tracer, Camera _camera)
{
	Ray ray;
	glm::vec3 pixelColour{ 1.0f, 0, 0 };
	for (int x = 0; x < _windowSize.x; x++) //iterate through every pixel on the screen and calculate its colour
	{
		for (int y = 0; y < _windowSize.y; y++)
		{
			ray = _camera.createRay(glm::ivec2(x, y)); //generates ray for specific pixel
			pixelColour = _tracer.trace(ray); //calculates colour of that pixel using the ray
			MCG::DrawPixel(glm::ivec2(x, y), pixelColour); //draws that pixel to the screen
		}
	}
}