#pragma once
#include <cmath>
#include "MCG_GFX_Lib.h"
#include <GLM/gtx/string_cast.hpp>
#include <iostream>
#include "Shapes.h"
#include <vector>
#include <cstdlib>

struct Material //these structs specify the values for different materials that can be used in the program
{
	glm::vec3 ambi;
	glm::vec3 diff;
	glm::vec3 spec;
	float shininess;
};

struct RedPlastic : public Material
{
	RedPlastic() 
	{
		ambi = glm::vec3(0.15f, 0.0f, 0.0f);
		diff = glm::vec3(0.5f, 0.0f, 0.0f);
		spec = glm::vec3(0.7f, 0.6f, 0.6f);
		shininess = 32;
	}
};

struct YellowPlastic : public Material
{
	YellowPlastic()
	{
		ambi = glm::vec3(0.15f, 0.15f, 0.0f);
		diff = glm::vec3(0.7f, 0.7f, 0.0f);
		spec = glm::vec3(0.7f, 0.7f, 0.6f);
		shininess = 32;
	}
};

