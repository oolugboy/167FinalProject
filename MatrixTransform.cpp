#include "MatrixTransform.h"
MatrixTransform::MatrixTransform()
{
	transformMatrix = glm::mat4(1.0f);
}
void MatrixTransform::draw(glm::mat4 cMatrix)
{ 
	Group::draw(cMatrix * transformMatrix);
}
void MatrixTransform::update()
{
	Group::update();
}
