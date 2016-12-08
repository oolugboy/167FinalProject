#include "Node.h"
#include <iostream>

using namespace std;

Node::Node()
{
	culled = false;	
}

void Node::printMatrix(glm::mat4 matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}
void Node::printVector(glm::vec3 vector)
{
	cout << vector.x << " " << vector.y << " " << vector.z << endl;
}
