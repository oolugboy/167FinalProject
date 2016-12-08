#include "City.h"


City::City()
{
	initial();
}


City::City(int intervals)
{
	this->world_intervals = intervals;
	initial();
}

void City::initial() {
	for (int z = 0; z < world_intervals; z++) {
		std::vector<int> horizonal;
		for (int x = 0; x < world_intervals; x++) {
			horizonal.push_back(-1);
		}
		this->occupation.push_back(horizonal);
	}
	computePoints();
	toWorld = glm::mat4(1.0f);

	cityShader = new Shader("shaders/city.vert", "shaders/city.frag");
	curveShader = new Shader("shaders/curve.vert", "shaders/curve.frag");

	generateVAO();
}

City::~City()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &EBO);
}

GLuint City::generateVAO() {
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texture_v.size() * sizeof(glm::vec2), &(texture_v[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	texture = loadTexture("textures/grass.jpg");

	return VAO;
}

bool City::addObject(glm::mat4 drawMatrix) {

	glm::vec3 temp_cube_vertices[4];
	for (int i = 0; i < 4; i++) {
		glm::vec4 new_point = drawMatrix * glm::vec4(cube_vertices[i], 1.0);
		temp_cube_vertices[i] = glm::vec3(new_point);
		if (temp_cube_vertices[i].x > world_intervals / 2.0f || temp_cube_vertices[i].x < world_intervals / -2.0f) {
			//printf("Out of Bound\n");
			return false;
		}
		if (temp_cube_vertices[i].z > world_intervals / 2.0f || temp_cube_vertices[i].z < world_intervals / -2.0f) {
			//printf("Out of Bound\n");
			return false;
		}
	}

	glm::vec3 new_cube_vertices[4];
	float mostLeftPos = -100000.0f;
	float mostTopPos = -100000.0f;
	float mostRightPos = 100000.0f;
	float mostBottomPos = 100000.0f;
	for (int i = 0; i < 4; i++) {
		if (mostLeftPos < temp_cube_vertices[i].x) {
			new_cube_vertices[0] = temp_cube_vertices[i];
			mostLeftPos = temp_cube_vertices[i].x;
		}
		else if (mostLeftPos == temp_cube_vertices[i].x && new_cube_vertices[0].z < temp_cube_vertices[i].z) {
			new_cube_vertices[0] = temp_cube_vertices[i];
		}

		if (mostTopPos < temp_cube_vertices[i].z) {
			new_cube_vertices[1] = temp_cube_vertices[i];
			mostTopPos = temp_cube_vertices[i].z;
		}
		else if (mostTopPos == temp_cube_vertices[i].z && new_cube_vertices[1].x > temp_cube_vertices[i].x) {
			new_cube_vertices[1] = temp_cube_vertices[i];
		}

		if (mostRightPos > temp_cube_vertices[i].x) {
			new_cube_vertices[2] = temp_cube_vertices[i];
			mostRightPos = temp_cube_vertices[i].x;
		}
		else if (mostRightPos == temp_cube_vertices[i].x && new_cube_vertices[2].z > temp_cube_vertices[i].z) {
			new_cube_vertices[2] = temp_cube_vertices[i];
		}

		if (mostBottomPos > temp_cube_vertices[i].z) {
			new_cube_vertices[3] = temp_cube_vertices[i];
			mostBottomPos = temp_cube_vertices[i].z;
		}
		else if (mostBottomPos == temp_cube_vertices[i].z && new_cube_vertices[3].x < temp_cube_vertices[i].x) {
			new_cube_vertices[3] = temp_cube_vertices[i];
		}
	}


	float m[6], b[6];

	m[0] = (new_cube_vertices[1].z - new_cube_vertices[0].z) / (new_cube_vertices[1].x - new_cube_vertices[0].x);
	//printf("z1 - z0 = %f - %f = %f\n", new_cube_vertices[1].z, new_cube_vertices[0].z, new_cube_vertices[1].z - new_cube_vertices[0].z);
	//printf("x1 - x0 = %f - %f = %f\n", new_cube_vertices[1].x, new_cube_vertices[0].x, new_cube_vertices[1].x - new_cube_vertices[0].x);
	//printf("m0 = (z1 - z0) / (x1 - x0) = %f = %f\n", m[0], (new_cube_vertices[1].z - new_cube_vertices[0].z) / (new_cube_vertices[1].x - new_cube_vertices[0].x));
	b[0] = (-1.0f * m[0] * new_cube_vertices[0].x + new_cube_vertices[0].z);

	m[1] = (new_cube_vertices[2].x - new_cube_vertices[1].x) / (new_cube_vertices[2].z - new_cube_vertices[1].z);
	b[1] = (-1.0f * m[1] * new_cube_vertices[1].z + new_cube_vertices[1].x);

	m[2] = (new_cube_vertices[3].z - new_cube_vertices[2].z) / (new_cube_vertices[3].x - new_cube_vertices[2].x);
	b[2] = (-1.0f * m[2] * new_cube_vertices[2].x + new_cube_vertices[2].z);

	m[3] = (new_cube_vertices[0].x - new_cube_vertices[3].x) / (new_cube_vertices[0].z - new_cube_vertices[3].z);
	b[3] = (-1.0f * m[3] * new_cube_vertices[3].z + new_cube_vertices[3].x);


	m[4] = (new_cube_vertices[2].z - new_cube_vertices[1].z) / (new_cube_vertices[2].x - new_cube_vertices[1].x);
	b[4] = (-1.0f * m[1] * new_cube_vertices[1].x + new_cube_vertices[1].z);

	m[5] = (new_cube_vertices[0].z - new_cube_vertices[3].z) / (new_cube_vertices[0].x - new_cube_vertices[3].x);
	b[5] = (-1.0f * m[3] * new_cube_vertices[3].x + new_cube_vertices[3].z);


	bool isOkay = true;
	int object_id = object_count + 1;
	//printf("  Add Object %d now (%3.3f, %3.3f), (%3.3f, %3.3f), (%3.3f, %3.3f), (%3.3f, %3.3f)\n", object_id,
	//	new_cube_vertices[0].x, new_cube_vertices[0].z,
	//	new_cube_vertices[1].x, new_cube_vertices[1].z,
	//	new_cube_vertices[2].x, new_cube_vertices[2].z,
	//	new_cube_vertices[3].x, new_cube_vertices[3].z);
	//printf("    Line 1 z <= %3.3f x + %3.3f\n", m[0], b[0]);
	//printf("    Line 2 x >= %3.3f z + %3.3f\n", m[1], b[1]);
	//printf("    Line 3 z >= %3.3f x + %3.3f\n", m[2], b[2]);
	//printf("    Line 4 x <= %3.3f z + %3.3f\n", m[3], b[3]);
	for (int z = 0; z < world_intervals && isOkay; z++) {
		for (int x = 0; x < world_intervals && isOkay; x++) {

			glm::vec3 p[17];
			p[0] = getPosition(x, z);			//left top
			p[1] = getPosition(x, z + 1);		//left bottom
			p[2] = getPosition(x + 1, z);		//right top
			p[3] = getPosition(x + 1, z + 1);	//right bottom

			p[4] = (p[0] + p[1]) / 2.0f;
			p[5] = (p[1] + p[2]) / 2.0f;
			p[6] = (p[2] + p[3]) / 2.0f;
			p[7] = (p[3] + p[0]) / 2.0f;

			p[8] = (p[0] + p[1] + p[2] + p[3]) / 4.0f;

			p[9] = (p[0] + p[4]) / 2.0f;
			p[10] = (p[1] + p[4]) / 2.0f;
			p[11] = (p[1] + p[5]) / 2.0f;
			p[12] = (p[2] + p[5]) / 2.0f;
			p[13] = (p[2] + p[6]) / 2.0f;
			p[14] = (p[3] + p[6]) / 2.0f;
			p[15] = (p[3] + p[7]) / 2.0f;
			p[16] = (p[0] + p[7]) / 2.0f;

			//printf("      Object %d: test point (%3.3f,%3.3f)\n", object_id, p[0].x, p[0].z);
			bool isOccured = false;
			for (int i = 0; i < 6; i++) {
				float x1 = p[i % 4].x;
				float z1 = p[i % 4].z;
				float x2 = p[((i % 4) + (i / 4) + 1) % 4].x;
				float z2 = p[((i % 4) + (i / 4) + 1) % 4].z;
				for (int j = 0; j <= 10; j++) {
					float xpos = x1 + (x2 - x1) * j / 10.0f;
					float zpos = z1 + (z2 - z1) * j / 10.0f;

					if (zpos <= (m[0] * xpos + b[0]) && //test for line 1, cube_p0 and cube_p1
						xpos >= (m[1] * zpos + b[1]) && //test for line 2, cube_p1 and cube_p2
						zpos >= (m[2] * xpos + b[2]) && //test for line 3, cube_p2 and cube_p3
						xpos <= (m[3] * zpos + b[3]))   //test for line 4, cube_p3 and cube_p0
					{
						isOccured = true;
						break;
					}
				}
			}

			if (isOccured) { //object occurs grid (x,z)
				if (occupation[z][x] != -1) { //grid (x,z) is occured by other object before
					isOkay = false;
					//printf("    Object %d: cannnot occured position (%d,%d) because object %d occured already \n", object_id, x, z, occupation[z][x]);
				}
				else {
					occupation[z][x] = object_id;
					//printf("    Object %d: occured position (%d,%d)\n", object_id, x, z);
				}
			}
			else {
				//printf("    Object %d: did not occured position (%d,%d)\n", object_id, x, z);

			}
		}

	}

	if (!isOkay) { //failure to add object, need to undo the mark for this object
		for (int z = 0; z < world_intervals; z++) {
			for (int x = 0; x < world_intervals; x++) {
				if (occupation[z][x] == object_id) {
					occupation[z][x] = -1; 
				}
			}
		}
	}
	else { //successful to add object, good to go
		object_count++;
		//printf("  add object %d successfully\n", object_id);
		//for (int z = 0; z < world_intervals; z++) {
		//	for (int x = 0; x < world_intervals; x++) {
		//		printf("%3d ", occupation[z][x]);
		//	}
		//	printf("\n");
		//}

		//printf("  Add Object %d now (%3.4f, %3.4f), (%3.4f,%3.4f), (%3.4f, %3.4f), (%3.4f, %3.4f)\n", object_id,
		//	new_cube_vertices[0].x, new_cube_vertices[0].z,
		//	new_cube_vertices[1].x, new_cube_vertices[1].z,
		//	new_cube_vertices[2].x, new_cube_vertices[2].z,
		//	new_cube_vertices[3].x, new_cube_vertices[3].z);
		//printf("    Line 1 z <= %3.4f x + %3.4f\n", m[0], b[0]);
		//printf("    Line 2 x >= %3.4f z + %3.4f\n", m[1], b[1]);
		//printf("    Line 3 z >= %3.4f x + %3.4f\n", m[2], b[2]);
		//printf("    Line 4 x <= %3.4f z + %3.4f\n", m[3], b[3]);

	}

	return isOkay;
}

void City::draw(glm::mat4 cMatrix) {

	cityShader->Use();
	glm::mat4 modelview = Window::V * cMatrix * toWorld ;

	glUniformMatrix4fv(glGetUniformLocation(cityShader->Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(cityShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));
	glBindVertexArray(VAO);

	if (Window::debug) {
		glUniform1i(glGetUniformLocation(cityShader->Program, "isTexture"), 0);
		glLineWidth(3.0f);
		//glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glPointSize(7.0f);
		glDrawArrays(GL_POINTS, 0, vertices.size());

		glDrawArrays(GL_LINES, 0, vertices.size());
	}

	glUniform1i(glGetUniformLocation(cityShader->Program, "isTexture"), 1);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(cityShader->Program, "tile"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindVertexArray(0);

	Group::draw(cMatrix * toWorld);

	//if (Window::debug) {
	//	drawCurves(cMatrix);
	//}
}


void City::drawCurves(Shader shader)
{
	for (int i = 0; i < curves_v.size(); i++) {
		curves_v[i]->draw(shader);
	}


	for (int i = 0; i < curves_h.size(); i++) {
		curves_h[i]->draw(shader);
	}
}

void City::drawCurves()
{
	for (int i = 0; i < curves_v.size(); i++) {
		curves_v[i]->draw(*curveShader);
	}


	for (int i = 0; i < curves_h.size(); i++) {
		curves_h[i]->draw(*curveShader);
	}
}

void City::drawCurves(glm::mat4 cMatrix)
{
	for (int i = 0; i < curves_v.size(); i++) {
		curves_v[i]->draw(*curveShader, cMatrix);
	}


	for (int i = 0; i < curves_h.size(); i++) {
		curves_h[i]->draw(*curveShader, cMatrix);
	}
}

void City::computePoints() {
	this->generateCurves();

	int z = 1;
	glm::vec3 v_pos, h_pos, pos0, pos1, pos2, pos3;
	int count = 0;
	for (int z = 0; z < world_intervals; z++) {
		for (int x = 0; x < world_intervals; x++) {
			v_pos = curves_v[world_intervals - x]->getPosition(z);
			h_pos = curves_h[world_intervals - z]->getPosition(x);
			pos0 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos0);
			texture_v.push_back(glm::vec2(0.0, 1.0));

			v_pos = curves_v[world_intervals - x]->getPosition(z + 1);
			h_pos = curves_h[world_intervals - z - 1]->getPosition(x);
			pos1 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos1);
			texture_v.push_back(glm::vec2(0.0, 0.0));

			v_pos = curves_v[world_intervals - x - 1]->getPosition(z);
			h_pos = curves_h[world_intervals - z]->getPosition(x + 1);
			pos2 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos2);
			texture_v.push_back(glm::vec2(1.0, 1.0));

			vertices.push_back(pos2);
			texture_v.push_back(glm::vec2(1.0, 1.0));
			vertices.push_back(pos1);
			texture_v.push_back(glm::vec2(0.0, 0.0));

			v_pos = curves_v[world_intervals - x - 1]->getPosition(z + 1);
			h_pos = curves_h[world_intervals - z - 1]->getPosition(x + 1);
			pos3 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos3);
			texture_v.push_back(glm::vec2(1.0, 0.0));

			//std::cout << "z=" << z << ", x=" << x << "th vertices: (" << pos.x << "," << pos.y << "," << pos.z << ")" << std::endl;
			//std::cout << "z=" << z << ", x=" << x << "th vertices: (" << v_pos.x << "," << v_pos.y << "," << v_pos.z << ")" << std::endl;
			//std::cout << "z=" << world_intervals - z << ", x=" << x << "th vertices: (" << h_pos.x << "," << h_pos.y << "," << h_pos.z << ")" << std::endl;
		}
	}


	for (int z = 0; z <= world_intervals; z++) {
		for (int x = 0; x <= world_intervals; x++) {
			glm::vec3 v_pos = curves_v[world_intervals - x]->getPosition(world_intervals - z);
			glm::vec3 h_pos = curves_h[z]->getPosition(x);
			glm::vec3 pos = (v_pos + h_pos) * 0.5f;
			city_grid.push_back(pos);
			//printf("(x,z) = (%d,%d) = (%f,%f)\n", x, z, pos.x, pos.z);
		}
	}

	//glm::mat4 pc = Window::P * Window::V;
	//for (int z = 0; z <= world_intervals; z++) {
	//	for (int x = 0; x <= world_intervals; x++) {
	//		glm::vec3 position = getPosition(x, z);
	//		glm::vec4 p = pc * glm::vec4(position, 1.0);
	//		float xpos = (Window::width * p.x / p.w + Window::width) / 2.0;
	//		float ypos = (Window::height * p.y / p.w + Window::height) / 2.0;


	//		printf("(x,z) = (%d,%d) = (%f,%f) = (%f,%f)\n", x, z, position.x, position.z, xpos, ypos);
	//	}
	//}

	//lines index
	//for (int z = 0; z <= world_intervals; z++) {
	//	for (int x = 0; x <= world_intervals; x++) {
	//		if (x != world_intervals) {
	//			indices.push_back(world_intervals*z + z + x);
	//			indices.push_back(world_intervals*z + z + x + 1);
	//		}
	//		if (z != world_intervals) {
	//			indices.push_back(world_intervals*z + z + x);
	//			indices.push_back(world_intervals*(z + 1) + z + 1 + x);
	//		}
	//	}
	//}
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(world_intervals + 1);
	//indices.push_back(world_intervals + 2);

	//for (int i = 0; i < vertices.size() - 1; i++) {
	//	indices.push_back(i);
	//	indices.push_back(i + 1);
	//}

}

void City::generateCurves() {

	int half = world_intervals / 2;
	int left = -1 * world_intervals / 2;
	int right = world_intervals / 2 + world_intervals % 2;

	float x_diff1, x_diff2, z_diff1, z_diff2;
	int x1_set_point = -half / 6;
	int x1_left = abs(x1_set_point - left);
	int x1_right = abs(x1_set_point - right);
	int x2_set_point = 3 * half / 5;
	int x2_left = abs(x2_set_point - left);
	int x2_right = abs(x2_set_point - right);
	int z1_set_point = -4 * half / 7;
	int z1_left = abs(z1_set_point - left);
	int z1_right = abs(z1_set_point - right);
	int z2_set_point = 2 * half / 5;
	int z2_left = abs(z2_set_point - left);
	int z2_right = abs(z2_set_point - right);

	//printf("Getting vertical lines\n");
	for (int i = 0; i <= world_intervals; i++) {
		int x = i - world_intervals / 2;
		//printf("  Getting vertical lines %d on x = %d\n", i, x);
		float z = (world_intervals / 4.0);
		float x1 = x - x1_set_point;
		float x2 = x - x2_set_point;
		if (x <= x1_set_point)
			x_diff1 = (world_intervals / 5.0) * (-1.0 * x1 * x1 + x1_left * x1_left) / (float)(x1_left * x1_left);
		else
			x_diff1 = (world_intervals / 5.0) * (-1.0 * x1 * x1 + x1_right * x1_right) / (float)(x1_right * x1_right);
		if (x <= x2_set_point)
			x_diff2 = (world_intervals / 3.0) * (-1.0 * x2 * x2 + x2_left * x2_left) / (float)(x2_left * x2_left);
		else
			x_diff2 = (world_intervals / 3.0) * (-1.0 * x2 * x2 + x2_right * x2_right) / (float)(x2_right * x2_right);

		float z1 = z - z1_set_point;
		float z2 = z - z2_set_point;
		if (z <= z1_set_point)
			z_diff1 = (world_intervals / 2.0) * (-1.0 * z1 * z1 + z1_left * z1_left) / (float)(z1_left * z1_left);
		else
			z_diff1 = (world_intervals / 2.0) * (-1.0 * z1 * z1 + z1_right * z1_right) / (float)(z1_right * z1_right);
		if (z <= x2_set_point)
			z_diff2 = (world_intervals / 4.0) * (-1.0 * z2 * z2 + z2_left * z2_left) / (float)(z2_left * z2_left);
		else
			z_diff2 = (world_intervals / 4.0) * (-1.0 * z2 * z2 + z2_right * z2_right) / (float)(z2_right * z2_right);

		std::vector<glm::vec3> cp = { { x, 0.0, right },{ x - x_diff1, 0.0, z + z_diff1 }  ,{ x + x_diff2, 0.0, -1 * z - z_diff2 }  ,{ x, 0.0, left } };  //vertical
		Curve* curve = new Curve(&cp, world_intervals);
		curve->generateVAO();
		curves_v.push_back(curve);
	}

	//printf("Getting horizontal lines\n");
	for (int i = 0; i <= world_intervals; i++) {
		int z = i - world_intervals / 2;
		//printf("  Getting horizontal lines %d on z = %d\n", i, z);
		float x = (world_intervals / 4.0);
		float x1 = x - x1_set_point;
		float x2 = x - x2_set_point;
		if (x <= x1_set_point)
			x_diff1 = (world_intervals / 3.0) * (-1.0 * x1 * x1 + x1_left * x1_left) / (float)(x1_left * x1_left);
		else
			x_diff1 = (world_intervals / 3.0) * (-1.0 * x1 * x1 + x1_right * x1_right) / (float)(x1_right * x1_right);
		if (x <= x2_set_point)
			x_diff2 = (world_intervals / 1.5) * (-1.0 * x2 * x2 + x2_left * x2_left) / (float)(x2_left * x2_left);
		else
			x_diff2 = (world_intervals / 1.5) * (-1.0 * x2 * x2 + x2_right * x2_right) / (float)(x2_right * x2_right);

		float z1 = z - z1_set_point;
		float z2 = z - z2_set_point;
		if (z <= z1_set_point)
			z_diff1 = (world_intervals / 3) * (-1.0 * z1 * z1 + z1_left * z1_left) / (float)(z1_left * z1_left);
		else
			z_diff1 = (world_intervals / 3) * (-1.0 * z1 * z1 + z1_right * z1_right) / (float)(z1_right * z1_right);
		if (z <= x2_set_point)
			z_diff2 = (world_intervals / 10.0) * (-1.0 * z2 * z2 + z2_left * z2_left) / (float)(z2_left * z2_left);
		else
			z_diff2 = (world_intervals / 10.0) * (-1.0 * z2 * z2 + z2_right * z2_right) / (float)(z2_right * z2_right);

		std::vector<glm::vec3> cp = { { right, 0.0, z },{ x - x_diff1, 0.0, z + z_diff1 }  ,{ -1 * x + x_diff2, 0.0, z - z_diff2 }  ,{ left, 0.0, z } };  //horizontal
		Curve* curve = new Curve(&cp, world_intervals);
		curve->generateVAO();
		curves_h.push_back(curve);
	}
}

glm::vec3 City::getPosition(float t) {

	glm::vec3 position;
	//int size = (int)(1.0f / step) * 3;
	//int j = (int)(t * size);
	//int r = j % 3;
	//j -= r;
	//position = vertices[(int)(t / step)];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}

glm::vec3 City::getPosition(int x, int z) {

	if (x > world_intervals || z > world_intervals) {
		return glm::vec3(-10000, -10000, -10000);
	}

	glm::vec3 position;
	//int j = index * 3;
	position = city_grid[world_intervals*z + z + x];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}



// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint City::loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}