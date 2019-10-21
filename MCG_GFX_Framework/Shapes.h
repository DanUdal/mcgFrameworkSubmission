#pragma once
#include <cmath>
#include "MCG_GFX_Lib.h"
#include <GLM/gtx/string_cast.hpp>
#include <cstdlib>
#include <vector>
#include "Materials.h"

struct LightSource
{
	glm::vec3 m_position;
	glm::vec3 m_color;
	LightSource(glm::vec3 _position, glm::vec3 _color) : m_position(_position), m_color(_color)
	{
		
	}
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 colour;
	Ray()
	{

	}
	Ray(glm::vec3 _origin, glm::vec3 _direction, glm::vec3 _colour = glm::vec3(1.0f,1.0f,1.0f)) : origin(_origin), direction(_direction), colour(_colour)
	{

	}
};

class shape //base virtual class used for the ray tracing vector
{
protected:
	Material* material;
public:
	virtual glm::ivec4 getCenter() = 0;
	virtual glm::vec3 intersect(Ray _ray) = 0;
	virtual glm::vec3 getColour() = 0;
	virtual glm::vec3 lighting(LightSource _light, glm::vec3 _intsec, std::vector<shape*> _shapes, Ray _ray) = 0;
};

class Line
{
private:
	glm::ivec2 m_start;
	glm::ivec2 m_end;
public:
	Line(glm::ivec2 _start, glm::ivec2 _end) : m_start(_start), m_end(_end)
	{

	}
	glm::ivec2 getStart()
	{
		return m_start;
	}
	glm::ivec2 getEnd()
	{
		return m_end;
	}
	void setStart(glm::ivec2 _start)
	{
		m_start = _start;
	}
	void setEnd(glm::ivec2 _end)
	{
		m_end = _end;
	}
	void draw();
};

class Square : public shape
{
private:
	glm::ivec4 m_center;
	int m_size;
public:
	Square(glm::ivec4 _center, float _size) : m_center(_center), m_size(_size)
	{

	}
	void draw();
	glm::ivec4 getCenter()
	{
		return m_center;
	}
	int getsize()
	{
		return m_size;
	}
	void setCenter(glm::ivec4 _center)
	{
		m_center = _center;
	}
	void setsize(int _size)
	{
		m_size = _size;
	}
};

class Triangle : public shape
{
private:
	glm::ivec2 m_vertices[3];
public:
	Triangle(glm::ivec2 _Vert1, glm::ivec2 _Vert2, glm::ivec2 _Vert3) : m_vertices{ _Vert1,_Vert2,_Vert3 }
	{

	}
	glm::ivec2 getVertex1()
	{
		return m_vertices[1];
	}
	glm::ivec2 getVertex2()
	{
		return m_vertices[2];
	}
	glm::ivec2 getVertex3()
	{
		return m_vertices[3];
	}
	void setVertex1(glm::ivec2 _Vert1)
	{
		m_vertices[1] = _Vert1;
	}
	void setVertex2(glm::ivec2 _Vert2)
	{
		m_vertices[2] = _Vert2;
	}
	void setVertex3(glm::ivec2 _Vert3)
	{
		m_vertices[3] = _Vert3;
	}
	void draw();
};

class Camera
{
private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 invproj;
	glm::mat4 invView;
public:
	Camera()
	{
		projection = glm::perspective(glm::radians(40.0f), 640.0f / 480.0f, 1.0f, 1000.0f);
		invproj = glm::inverse(projection);
		view = glm::mat4(1);
		invView = glm::inverse(view);
	}
	Ray createRay(glm::ivec2 _position);
};

class RayTracer
{
private:
	std::vector<shape*> shapes; //vector of pointers to the base class to allow for different shapes to be used
	LightSource m_light;
public:
	RayTracer() : m_light(glm::vec3(0, 100, -600), glm::vec3(1.0f, 1.0f, 1.0f)) //initialising position and colour of light
	{
		
	}
	void addShape(shape *_shape)
	{
		shapes.push_back(_shape); //adds a shape to the vector
	}
	glm::vec3 trace(Ray _ray);
	int closest(Ray _ray);
};

class sphere : public shape
{
private:
	glm::vec4 m_center;
	float m_radius;
	glm::vec3 m_color;
public:
	sphere(float _radius, glm::vec4 _center, glm::vec3 _color, Material* _material = nullptr) 
		: m_center(_center), m_radius(_radius), m_color(_color)
	{
		material = _material; //initialises material center and radius of sphere
	}
	glm::vec3 draw(Ray _ray, glm::ivec3 _point);
	glm::vec3 intersect(Ray _ray);
	glm::ivec4 getCenter() //getter
	{
		return m_center;
	}
	float getRadius() //getter
	{
		return m_radius;
	}
	void setCenter(glm::ivec4 _center) //setter
	{
		m_center = _center;
	}
	void setRadius(float _radius) //setter
	{
		m_radius = _radius;
	}
	glm::vec3 getColour()
	{
		return m_color;
	}
	void transform(glm::mat4 _transformation, glm::ivec4 _point);
	glm::vec3 lighting(LightSource _light, glm::vec3 _intsec, std::vector<shape*> _shapes, Ray _ray);
};

glm::vec3 sphere::draw(Ray _ray, glm::ivec3 _point)
{
	/*glm::vec3 pixelColour{ m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f };
	glm::vec3 ambience{ 0.1f * pixelColour.r, 0.1f * pixelColour.g, 0.1f * pixelColour.b };
	float colorDepth1;
	glm::ivec2 pixelPosition = { 0,0 };
	for (int x = 0; x < (m_center.x + m_radius); x++)
	{
		for (int y = 0; y < (m_center.y + m_radius); y++)
		{
			if (_CMATH_::pow(x - (m_center.x), 2) + _CMATH_::pow(y - (m_center.y), 2) <= _CMATH_::pow(m_radius, 2))
			{
				pixelPosition = glm::ivec2(x, y);
				glm::vec2 newVec1 = pixelPosition - _light.m_position;
				colorDepth1 = _CMATH_::sqrt(_CMATH_::pow(newVec1.x, 2) + _CMATH_::pow(newVec1.y, 2));
				glm::cross(_light.m_color, pixelColour);
				pixelColour = pixelColour + ambience;
				if (pixelColour.r < 0)
				{
					pixelColour.r = 0;
				}
				if (pixelColour.b < 0)
				{
					pixelColour.b = 0;
				}
				if (pixelColour.g < 0)
				{
					pixelColour.g = 0;
				}
				if (pixelColour.r > 1.0f)
				{
					pixelColour.r = 1;
				}
				if (pixelColour.b > 1.0f)
				{
					pixelColour.b = 1;
				}
				if (pixelColour.g > 1.0f)
				{
					pixelColour.g = 1;
				}
				pixelColour = { 255 * pixelColour[0], 255 * pixelColour[1], 255 * pixelColour[2] };
				MCG::DrawPixel(pixelPosition, pixelColour);
			}
		}

	}*/
	return glm::vec3(1.0f, 0.0f, 0.0f);
} //currently unused function for drawing 2D circle

glm::vec3 sphere::intersect(Ray _ray)
{
	glm::vec3 point{ m_center.x, m_center.y, m_center.z }; //changing center from 4d to 3d for the formulae in this function
	if (abs(glm::distance(_ray.origin, point)) < m_radius)
	{
		return glm::vec3(-1, -1, 1); //return false if ray starts inside sphere
	}
	glm::vec3 test = _ray.origin + (glm::max(0.0f, glm::dot(glm::vec3(point - _ray.origin), _ray.direction)) *_ray.direction);
	float dist = abs(glm::distance(test, point));
	if (dist > m_radius)
	{
		return glm::vec3(-1, -1, 1); //return false if ray doesn't intersect sphere
	}
	float x(_CMATH_::sqrt(_CMATH_::pow(m_radius, 2) - _CMATH_::pow(dist, 2)));
	glm::vec3 intsec = _ray.origin + (((glm::max(0.0f, glm::dot(glm::vec3(point - _ray.origin), _ray.direction))) - x) *_ray.direction); //calculate distance from origin to intersection
	/*if (abs(glm::distance(intsec, _ray.origin)) < (m_center.z * -1) - m_radius)
	{
		return ((m_center.z * -1) - m_radius);
	}*/
	if (glm::distance(intsec, _ray.origin) < 0)
	{
		return glm::vec3(-1, -1, 1); //return false if the object is behind the camera
	}
	return intsec; //return intersection point
}

glm::vec3 sphere::lighting(LightSource _light, glm::vec3 _intsec, std::vector<shape*> _shapes, Ray _ray)
{
	glm::vec3 normal;
	glm::vec3 lightVec;
	float angle;
	glm::vec3 colour;
	glm::vec3 reflect;
	glm::vec3 camera;
	glm::vec3 half;
	glm::vec3 temp(-1.0f, -1.0f, 1.0f); //creating variables for lighting function
	camera = glm::normalize(-_intsec); //initialising camera position
	lightVec =  glm::normalize(_light.m_position - _intsec); //getting vector from point to light source
	Ray lightRay(_intsec, lightVec); //creating ray for shadow testing
	colour = material->ambi * _light.m_color; //add colour for simulating ambient light
	for (int i = 0; i < size(_shapes); i++)
	{
		if (_shapes.at(i) != this)
		{
			temp = _shapes.at(i)->intersect(lightRay); //check for anything obstructing the light
		}
		if ((temp.x > 0.0f) && (temp.y > 0.0f) && (temp.z < 0.0f))
		{
			i = size(_shapes);
		}
	}
	normal = glm::normalize(_intsec - glm::vec3(m_center.x, m_center.y, m_center.z)); //calculate normal vector for point
	//half = glm::normalize((camera + lightVec) / glm::distance(_intsec, (camera + lightVec)));
	angle = glm::max(0.0f, glm::dot(normal, lightVec)); //calculate cos of angle between normal and light ray
	colour += material->diff * _light.m_color * angle; //add diffuse component of light to the colour
	if (angle > 0.0f) //calculate specular only if the normal is towards the light source
	{
		reflect = glm::normalize((2.0f * (glm::max(0.0f, glm::dot(normal, lightVec)) * normal)) - lightVec); //calculate reflection vector
		angle = glm::max(0.0f, glm::dot(camera, reflect)); //calculate cos of angle between reflection and view direction
		if (angle > 0.0f)
		{
			colour += (_CMATH_::pow(angle, material->shininess)) * _light.m_color * material->spec; //add specular component to colour
		}
	}
	if ((temp.x > 0.0f) && (temp.y > 0.0f) && (temp.z < 0.0f))
	{
		colour /= 2; //halves the colour if the object is in shadow
	}
	return colour;
}

void Square::draw() //function for drawing 2D square. curently unused
{
	glm::ivec2 start, end;
	glm::ivec3 pixelColour{ 255, 0, 0 };
	float colorDepth1, colorDepth2;
	glm::ivec2 lightPos1{ 50,25 }, lightPos2{ 635,475 };
	glm::ivec2 pixelPosition;
	start = m_center - (m_size / 2);
	end = start;
	end += m_size;
	for (int x = 0; x < end.x; x++)
	{
		for (int y = 0; y < end.y; y++)
		{
			if ((x > start.x) && (y > start.y))
			{
				glm::ivec2 newVec1 = glm::ivec2(x, y) - lightPos1;
				colorDepth1 = _CMATH_::sqrt(_CMATH_::pow(newVec1.x, 2) + _CMATH_::pow(newVec1.y, 2));
				glm::ivec2 newVec2 = glm::ivec2(x, y) - lightPos2;
				colorDepth2 = _CMATH_::sqrt(_CMATH_::pow(newVec2.x, 2) + _CMATH_::pow(newVec2.y, 2));
				pixelColour = { 255, 230, 0 };
				if (colorDepth1 > colorDepth2)
				{
					pixelColour -= 0.6*colorDepth2;
				}
				else
				{
					pixelColour -= 0.6*colorDepth1;
				}
				if (pixelColour.r < 0)
				{
					pixelColour.r = 0;
				}
				if (pixelColour.b < 0)
				{
					pixelColour.b = 0;
				}
				if (pixelColour.g < 0)
				{
					pixelColour.g = 0;
				}
				if (pixelColour.r > 255)
				{
					pixelColour.r = 255;
				}
				if (pixelColour.b > 255)
				{
					pixelColour.b = 255;
				}
				if (pixelColour.g > 255)
				{
					pixelColour.g = 255;
				}
				MCG::DrawPixel(glm::ivec2(x, y), pixelColour);
			}
		}
	}
}

void Triangle::draw() //function for drawing 2D triangle
{
	glm::ivec3 pixelColour{ 255, 0, 0 };
	float colorDepth;
	glm::ivec2 lightPos{ 500,460 };
	glm::ivec2 pixelPosition;
	float grad1(((float)m_vertices[1].y - (float)m_vertices[2].y) / ((float)m_vertices[1].x - (float)m_vertices[2].x));
	float grad2(((float)m_vertices[1].y - (float)m_vertices[3].y) / ((float)m_vertices[1].x - (float)m_vertices[3].x));
	float grad3(((float)m_vertices[3].y - (float)m_vertices[2].y) / ((float)m_vertices[3].x - (float)m_vertices[2].x));
	float intercept1 = m_vertices[1].y - (grad1 * m_vertices[1].x);
	float intercept2 = m_vertices[1].y - (grad2 * m_vertices[1].x);
	float intercept3 = m_vertices[2].y - (grad1 * m_vertices[2].x);
	for (int x = 0; x < m_vertices[3].x; x++)
	{
		for (int y = 0; y < m_vertices[3].y; y++)
		{
			if ((y > ((grad1*x) + intercept1)) && (y > ((grad2*x) + intercept2)) && (y < ((grad3*x) + intercept3)))
			{
				glm::ivec2 newVec = glm::ivec2(x, y) - lightPos;
				colorDepth = _CMATH_::sqrt(_CMATH_::pow(newVec.x, 2) + _CMATH_::pow(newVec.y, 2));
				pixelColour = { 255 - 0.3*colorDepth, 255 - 0.3*colorDepth, 255 - 0.3*colorDepth };
				if (pixelColour.r < 0)
				{
					pixelColour.r = 0;
				}
				if (pixelColour.b < 0)
				{
					pixelColour.b = 0;
				}
				if (pixelColour.g < 0)
				{
					pixelColour.g = 0;
				}
				if (pixelColour.r > 255)
				{
					pixelColour.r = 255;
				}
				if (pixelColour.b > 255)
				{
					pixelColour.b = 255;
				}
				if (pixelColour.g > 255)
				{
					pixelColour.g = 255;
				}
				MCG::DrawPixel(glm::ivec2(x, y), pixelColour);
			}
		}
	}
}

void Line::draw() //function for drawing 2D line
{
	glm::ivec3 pixelColour{ 0, 255, 0 };
	glm::ivec2 pixelPosition = m_start;
	if (m_start.x - m_end.x == 0)
	{
		if (m_start.y < m_end.y)
		{
			while (pixelPosition.y < m_end.y)
			{
				MCG::DrawPixel(pixelPosition, pixelColour);
				pixelPosition.y += 1;
				MCG::ProcessFrame();
			}
		}
		else
		{
			while (pixelPosition.y > m_end.y)
			{
				MCG::DrawPixel(pixelPosition, pixelColour);
				pixelPosition.y -= 1;
				MCG::ProcessFrame();
			}
		}
	}
	else
	{
		float grad = ((float)m_start.y - (float)m_end.y) / ((float)m_start.x - (float)m_end.x);
		float intercept = m_start.y - (grad * m_start.x);
		if (grad > 0)
		{
			if (pixelPosition.y < m_end.y)
			{
				while (pixelPosition.y < m_end.y)
				{
					MCG::DrawPixel(pixelPosition, pixelColour);
					pixelPosition.x += 1;
					pixelPosition.y = (grad * pixelPosition.x) + intercept;
					MCG::ProcessFrame();
				}
			}
			else
			{
				while (pixelPosition.y > m_end.y)
				{
					MCG::DrawPixel(pixelPosition, pixelColour);
					pixelPosition.x -= 1;
					pixelPosition.y = (grad * pixelPosition.x) + intercept;
					MCG::ProcessFrame();
				}
			}
		}
		else if (grad < 0)
		{
			if (pixelPosition.y > m_end.y)
			{
				while (pixelPosition.y > m_end.y)
				{
					MCG::DrawPixel(pixelPosition, pixelColour);
					pixelPosition.x += 1;
					pixelPosition.y = (grad * pixelPosition.x) + intercept;
					MCG::ProcessFrame();
				}
			}
			else
			{
				while (pixelPosition.y < m_end.y)
				{
					MCG::DrawPixel(pixelPosition, pixelColour);
					pixelPosition.x -= 1;
					pixelPosition.y = (grad * pixelPosition.x) + intercept;
					MCG::ProcessFrame();
				}
			}
		}
		else if (m_start.x < m_end.x)
		{
			while (pixelPosition.x < m_end.x)
			{
				MCG::DrawPixel(pixelPosition, pixelColour);
				pixelPosition.x += 1;
				MCG::ProcessFrame();
			}
		}
		else
		{
			while (pixelPosition.x > m_end.x)
			{
				MCG::DrawPixel(pixelPosition, pixelColour);
				pixelPosition.x -= 1;
				MCG::ProcessFrame();
			}
		}
	}
}

void sphere::transform(glm::mat4 _transformation, glm::ivec4 _point)
{
	m_center = _transformation * m_center; //performs a transformation of the sphere
}

glm::vec3 RayTracer::trace(Ray _ray)
{	
	glm::vec3 intersect(-1, -1, 1); //-1 used as false
	glm::vec3 colour{ 1.0f, 1.0f, 1.0f }; //default colour for no intersection
	int i = 0;
	i = this->closest(_ray);
	if (i > -1)
	{
		intersect = shapes.at(i)->intersect(_ray); //check for intersection
	}
	if ((intersect.x > 0.0f) && (intersect.y > 0.0f) && (intersect.z < 0.0f))
	{
		colour = shapes.at(i)->lighting(m_light, intersect, shapes, _ray); //calculate base colour for pixel
		i = size(shapes);
		if (colour.r < 0.1f)
				{
					colour.r = 0.1f;
				}
		if (colour.b < 0.1f)
				{
					colour.b = 0.1f;
				}
		if (colour.g < 0.1f)
				{
					colour.g = 0.1f;
				}
		if (colour.r > 1.0f)
				{
					colour.r = 1;
				}
		if (colour.b > 1.0f)
				{
					colour.b = 1;
				}
		if (colour.g > 1.0f)
				{
					colour.g = 1;
				}
	} //deal with any possible overflow error
	return colour * 255.0f; //return denormalised colour for drawing
}

Ray Camera::createRay(glm::ivec2 _position)
{
	Ray ray;
	glm::vec4 near, far;
	near = { (_position.x / 640.0f) /*- 1.0f*/, (_position.y / 480.0f) /*- 1.0f*/,-1.0f,1.0f };
	far = near; //generate positions on the near and far plane in NDC space
	far.z = 1.0f; //set far z co-ordinate to 1 for the far plane
	near = invproj * near; 
	far = invproj * far; //change from NDC space to world space
	near = near / near.w;
	far = far / far.w; //renormalise near and far after projection transformation
	near = invView * near;
	far = invView * far; //transform for camera position
	ray.origin = near;
	ray.direction = glm::normalize(far - near); //set ray origin and normalised direction
	return ray;
}

int RayTracer::closest(Ray _ray)
{
	int shape(-1); //defaults to -1 for no collision
	glm::vec3 intsec;
	float distance(2000);
	for (int i = 0; i < size(shapes); i++) //tests distance to collision for every object in the scene
	{
		intsec = shapes.at(i)->intersect(_ray); //returns any intersection point
		if ((distance > glm::distance(_ray.origin, intsec)) && (intsec != glm::vec3(-1,-1,1)))
		{
			distance = glm::distance(_ray.origin, intsec); //replaces distance if current object is closer
			shape = i; //returns index for the closest shape
		}
	}
	return shape;
}