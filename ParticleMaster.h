#ifndef _PARTICALMASTER_H_
#define _ParticleMaster_H_


#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <limits>
#include <list>
#include "Group.h"  
#include "Particle.h"  
#include <typeinfo>


using namespace std;
class ParticleMaster : public Group
{
public:
	ParticleMaster() {};
	bool addChild(Particle * child);
	bool removeChild(Particle * child);
	void update();
	void draw(glm::mat4 cMatrix);
	std::vector< Particle * > childrens;
};
#endif