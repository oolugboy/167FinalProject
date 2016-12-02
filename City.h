#ifndef _CITY_H_
#define _CITY_H_

#include "Curve.h"
#include "Node.h"
#include "Shader.h"

class City
{
public:
	City();
	~City();

	GLuint generateVAO();


	bool addObject(Node *object, glm::mat4 drawMatrix);

	void draw(Shader shader);
	void drawCurves(Shader shader);
	void update();

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

	
	//used to check new add object to city is okay or not
	Shader *occupationShader;
	std::vector<std::vector<bool>> occupation;


	//used to generate curves, then generate vertices
	std::vector<Curve*> curves_v;
	std::vector<Curve*> curves_h;
	int world_intervals;
};

#endif