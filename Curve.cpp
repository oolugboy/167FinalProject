#include "Curve.h"
#include "Window.h"


Curve::Curve(std::vector<glm::vec3>* cp, int interval = 200)
{
	this->interval = interval;
	this->step = 1.0f / (float)interval;
	control_points = cp;
	computePoints();
	toWorld = glm::mat4(1.0f);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

Curve::~Curve()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Curve::generateVAO() 
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &(vertices[0]), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}

void Curve::draw(Shader shader)
{
	glm::mat4 modelview = Window::V * toWorld;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));
	glBindVertexArray(VAO);

	glLineWidth(3.0f);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	//glPointSize(10.0f);
	//glDrawArrays(GL_POINTS, 0, vertices.size());
	//glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glBindVertexArray(0);
}

void Curve::draw(Shader shader, glm::mat4 cMatrix)
{
	glm::mat4 modelview = Window::V * cMatrix * toWorld;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));
	glBindVertexArray(VAO);

	glLineWidth(3.0f);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	//glPointSize(10.0f);
	//glDrawArrays(GL_POINTS, 0, vertices.size());
	//glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glBindVertexArray(0);
}

void Curve::update()
{

}

void Curve::computePoints() {
	std::vector<glm::vec3> &cps = *control_points;
	glm::vec3 p0(cps[0]);
	glm::vec3 p1(cps[1]);
	glm::vec3 p2(cps[2]);
	glm::vec3 p3(cps[3]);
	int j = 0;
	float t = 0.0f;
	for (; j <= interval; t += step, j++) {
		float tr = (1 - t);
		float c0 = tr * tr * tr;
		float c1 = 3.0f * t * tr * tr;
		float c2 = 3.0f * t * t * tr;
		float c3 = t * t * t;

		glm::vec3 q = c0 * p0 + c1 * p1 + c2 * p2 + c3 * p3;
		vertices.push_back(q);
		//std::cout << j << "th vertices: (" << vertices[j].x << "," << vertices[j].y << "," << vertices[j].z << ")" << std::endl;
	}
		//std::cout << "  (index: " << i*size + j * 3 << ", t: " << t << ", step: " << step << ")" << std::endl;
	

	//std::cout << "0th vertices: (" << vertices[0].x << "," << vertices[0].y << "," << vertices[0].z << ")" << std::endl;
	//std::cout << vertices.size() - 1 << "th last vertices: (" << vertices[vertices.size()-1].x << "," << vertices[vertices.size() - 1].y << "," << vertices[vertices.size() - 1].z << ")" << std::endl;
	//std::cout << "control_points size: " << cps.size() << ", vertices size: " << vertices.size()/3 << std::endl;

	//for (int i = 0; i < vertices.size() / 3 - 1; i++) {
	for (int i = 0; i < vertices.size() - 1; i++) {
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	
}

glm::vec3 Curve::getPosition(float t) {

	glm::vec3 position;
	int size = (int)(1.0f / step) * 3;
	int j = (int)(t * size);
	int r = j % 3;
	j -= r;
	position = vertices[(int)(t / step)];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}

glm::vec3 Curve::getPosition(int index) {

	if (index > interval) {
		return glm::vec3(-10000, -10000, -10000);
	}

	glm::vec3 position;
	int j = index * 3;
	position = vertices[index];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}



