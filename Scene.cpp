#include "Scene.h"

using namespace std;


float PI = 3.14159265;
float modder = pow(10, 9) + 7;

Scene::Scene(int numRobots, GLint shaderProgram1, GLint shaderProgram2)
{
	/* Initialize the degree for walking */
	deg = 1.0f;
	armyRotDeg = 5.0f;
	show = true;
	m_numRobots = numRobots;
	m_shaderProgram1 = shaderProgram1;
	m_shaderProgram2 = shaderProgram2;
	skyBox = new SkyBox();
	worldGroup = new Group();		
	boundBoxATrans = new MatrixTransform();
	boundBoxBTrans = new MatrixTransform();
	sphereATrans = new MatrixTransform();
	sphereBTrans = new MatrixTransform();	
	
	genSphere = new Sphere(1.0f, false);	
	boundCubeA = new Cube(true);
	boundCubeB = new Cube(true);
	buildGraph();
	/* Initialize the sizes */
	initializeObjects();

	/* SHAPE GRAMMAR TESTING*/
	BuildingGrammar * buildingGram = new BuildingGrammar();
	buildingGram->Build();
}
void Scene::draw()
{
	// Use the first shader program to draw the skybox
	glUseProgram(m_shaderProgram1);
	/* We first draw the skybox */
	skyBox->draw(m_shaderProgram1);	
	/** Now to use */
	glUseProgram(m_shaderProgram2);
	clock_t t;
	t = clock();
	worldGroup->draw(glm::mat4(1.0f));
	t = clock() - t;
	//cout << " It took this " << t << " clicks to render the robots in this frame " << endl;
	worldGroup->update();
}

void Scene::mouseOrbit(glm::vec3 & lastPosition, glm::vec3 & currPosition, glm::vec3 & cam_pos, int width, int height)
{
	glm::vec3 mappedLastPos = trackBallMapping(lastPosition, width, height);
	glm::vec3 mappedCurrPos = trackBallMapping(currPosition, width, height);

	float mLPMag = glm::length(mappedLastPos);
	float mCPMag = glm::length(mappedCurrPos);

	glm::vec3 direction = mappedCurrPos - mappedLastPos;
	float velocity = glm::length(direction);
	if (velocity > 0.001)
	{
		glm::vec3 rotAxis = glm::cross(mappedLastPos, mappedCurrPos);
		float rotAngle = acos(glm::dot(mappedLastPos, mappedCurrPos) / (mLPMag * mCPMag));

		glm::vec4 temp = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * glm::vec4(cam_pos, 1.0f);
		cam_pos = glm::vec3(temp.x, temp.y, temp.z);
		lastPosition = currPosition;
	}
}
glm::vec3 Scene::trackBallMapping(glm::vec3 point, int width, int height)
{
	glm::vec3 res;
	res.x = ((2.0f * point.x) - width) / width;
	res.y = (height - (2.0f * point.y)) / height;
	res.z = 0.0f;
	float depth = fmin(glm::length(res), 1.0f);
	res.z = sqrt(1.001f - pow(depth, 2));
	res = glm::normalize(res);
	return res;
}
void Scene::moveBalls()
{	
	sphereATrans->transformMatrix = sphereATrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(sin((deg * PI)/180.0f), 0.0f, 0.0f));
	sphereBTrans->transformMatrix = sphereBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(cos(((deg * PI)/180.0f) + PI/2.0f), 0.0f, 0.0f));

}
void Scene::buildGraph()
{
	/* Now to build the tree */	
	worldGroup->addChild(sphereATrans);	
	worldGroup->addChild(sphereBTrans);

	sphereATrans->addChild(genSphere);
	sphereBTrans->addChild(genSphere);

	sphereATrans->addChild(boundBoxATrans);
	sphereBTrans->addChild(boundBoxBTrans);

	boundBoxATrans->addChild(boundCubeA);
	boundBoxBTrans->addChild(boundCubeB);
}
void Scene::initializeObjects()
{
	sphereATrans->transformMatrix = sphereATrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, 0.0f, 0.0f));
	sphereATrans->transformMatrix = sphereATrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	sphereBTrans->transformMatrix = sphereBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.0f, 0.0f));
	sphereBTrans->transformMatrix = sphereBTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}
bool Scene::isCollide()
{
	return boundCubeA->collides(boundCubeB);
}
void Scene::zoom(float scrollOffset, glm::vec3 & cam_pos)
{
	float fact = 0.0f;
	if (scrollOffset > 0)
		fact = 1.5f;
	if (scrollOffset < 0)
		fact = 0.75f;
}

Scene::~Scene()
{
	delete(skyBox);
	delete(worldGroup);	
}