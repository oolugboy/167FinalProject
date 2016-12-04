#ifndef _CITY_H_
#define _CITY_H_

#include "Curve.h"
//#include "Node.h"
//#include "Geode.h"
#include "Group.h"
#include "Shader2.h"
#include <SOIL.h>
//#include "something.h"

class City : public Group
{
public:
	City();
	City(int intervals);
	void initial();
	~City();

	GLuint generateVAO();


	bool addObject(Node *object, glm::mat4 drawMatrix);

	void draw(glm::mat4 cMatrix);
	void drawCurves(Shader shader);
	void drawCurves();

	void computePoints();
	void generateCurves();


	glm::vec3 getPosition(float t);
	glm::vec3 getPosition(int x, int z);

	GLuint loadTexture(GLchar * path);


	// These variables are needed for the shader program
	GLuint VBO, VAO, TBO, EBO;
	GLuint uProjection, uModelview, texture;

	glm::mat4 toWorld;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texture_v;
	std::vector<glm::vec3> city_grid;
	std::vector<int> indices;

	//shader for this object
	Shader* cityShader;
	Shader* curveShader; //may not need it

						 //used to check new add object to city is okay or not
	std::vector<std::vector<bool>> occupation;


	//used to generate curves, then generate vertices
	std::vector<Curve*> curves_v;
	std::vector<Curve*> curves_h;
	int world_intervals;
};

#endif