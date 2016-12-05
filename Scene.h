#ifndef _SCENE_H_
#define _SCENE_H_


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
#include "SkyBox.h"
#include "Cube.h"
#include "Sphere.h"
#include "MatrixTransform.h"
#include "Ball.h"
#include <string>
#include <time.h>

//Building Grammar
#include "BuildingGrammar.h"

class Scene
{
	public:
		int m_numRobots;
		GLint m_shaderProgram1;
		GLint m_shaderProgram2;
		Scene(int numRobots, GLint shaderProgram1, GLint shaderProgram2);
		void draw();
		~Scene();
		SkyBox * skyBox;
		float deg;
		float armyRotDeg;
		void zoom(float scrollOffset, glm::vec3 & cam_pos);
		void mouseOrbit(glm::vec3 & lastPosition, glm::vec3 & currPosition, glm::vec3 & cam_pos, int width, int height);	
		glm::vec3 trackBallMapping(glm::vec3 point, int width, int height);
		Group * worldGroup;		
		MatrixTransform * boundBoxATrans;
		MatrixTransform * boundBoxBTrans;
		MatrixTransform * playerBallTrans;
		MatrixTransform * ballBTrans;
		vector <glm::mat4 > origPos;
		glm::vec3 playerTrans;
		glm::vec3 aITrans;
		glm::mat4 worldMatTrans;
		Ball * player;
		Ball * aI;
		void buildGraph();
		void initializeObjects();
		void moveBalls();	
		bool isCollide();
		void changePlayerDirection(float direction, bool posAccel);
		Sphere * genSphere;
		Cube * genCube;
		vector < Ball * > gameBalls;
		void acceleratePlayer(bool posAccel);
		void jumpPlayer(bool accel);
		vector< Object * > collidableObjects;		
		clock_t t;
};
#endif
