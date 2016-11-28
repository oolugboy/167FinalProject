#ifndef _GROUP_H_
#define _GROUP_H_


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
#include "Node.h"  


using namespace std;
class Group : public Node
{
public:
	Group() {};
	void draw(glm::mat4 cMatrix);
	void update();	
	bool addChild(Node * child);
	bool removeChild(Node * child);
	list< Node * > children;	
};
#endif