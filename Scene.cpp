#include "Scene.h"

using namespace std;


float PI = 3.14159265;
float modder = pow(10, 9) + 7;
float defaultAccel = 0.01f;

/*~~ SHAPE GRAMMAR TESTING*/
MatrixTransform * buildingTrans;
/*~~ END */


Scene::Scene(int numRobots, GLint shaderProgram1, GLint shaderProgram2)
{
	/* Initialize the required variables */
	t = clock();
	m_shaderProgram1 = shaderProgram1;
	m_shaderProgram2 = shaderProgram2;
	skyBox = new SkyBox();
	worldGroup = new Group();		
	boundBoxATrans = new MatrixTransform();
	boundBoxBTrans = new MatrixTransform();
	playerBallTrans = new MatrixTransform();
	ballBTrans = new MatrixTransform();	

	/* The general relevant geometries */
	genSphere = new Sphere(1.0f, false);
	genCube = new Cube(true);

	player = new Ball(true, glm::vec3(0,0,0));	
	player->sphere = genSphere;	

	aI = new Ball(false, glm::vec3(0, 0, -25.0f));
	ballBTrans->transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -25.0f));
	aI->sphere = genSphere;

	city = new City(100);

	/*~~ SHAPE GRAMMAR TESTING*/
	BuildingGrammar * buildingGram = new BuildingGrammar();
	buildingTrans = buildingGram->Build(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 45.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 20.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 80.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 30.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(2.0f, 0.0f, 5.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f);
	worldGroup->addChild(buildingTrans);
	buildingTrans = buildingGram->Build(glm::vec3(-2.0f, 0.0f, 5.0f), glm::vec3(0.5f, 0.5f, 0.5f), 68.0f);
	worldGroup->addChild(buildingTrans);
	/*~~ END */

	buildGraph();
	/* Initialize the sizes */
	initializeObjects();

	
}
void Scene::draw()
{
	// Use the first shader program to draw the skybox
	glUseProgram(m_shaderProgram1);
	/* We first draw the skybox */
	skyBox->draw(m_shaderProgram1);	
	/** Now to use */
	glUseProgram(m_shaderProgram2);
	worldGroup->draw(glm::mat4(1.0f));
	

	/* Test the collision detection */
	if (isCollide())
		cout << " Collision detected " << endl;

	//cout << " It took this " << t << " clicks to render the robots in this frame " << endl;
	worldGroup->update();
	city->drawCurves();

}
void Scene::changePlayerDirection(float direction, bool posAccel)
{
	player->turn = direction;		
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
/* Reset the acceleration */
void Scene::acceleratePlayer(bool posAccel)
{
	player->accelerate(posAccel);
	aI->accelerate(false);
}
void Scene::moveBalls()
{	 
	/* The amount of time before the previous call*/
	float time = (float)(clock() - t) / CLOCKS_PER_SEC;
	t = clock();
	
	/* The new distance that the ball should move in this frame */
	//S = ut + 1/2(a(t)^2)	
	/*cout << " The acceleration is ";
	player->printVector(player->acceleration);*/
	glm::vec3 diff = glm::vec3(0.0f, 0.0f, 0.0f);
	diff = (player->initVelocity * time) + (0.50f * player->acceleration * pow(time, 2));
	cout << " The diff is ";
	player->printVector(diff);	
	playerBallTrans->transformMatrix = playerBallTrans->transformMatrix * glm::translate(glm::mat4(1.0f), diff);
	
	/* For the aI part */
	diff = (aI->initVelocity * time) + (0.50f * aI->acceleration * pow(time, 2));
	cout << " The aI acceleration is ";
	player->printVector(aI->acceleration);
	ballBTrans->transformMatrix = ballBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), diff); 
}
void Scene::buildGraph()
{
	/* Now to build the tree */	
	worldGroup->addChild(playerBallTrans);	
	worldGroup->addChild(ballBTrans);
	worldGroup->addChild(city);

	playerBallTrans->addChild(player);
	ballBTrans->addChild(aI);

	playerBallTrans->addChild(boundBoxATrans);
	ballBTrans->addChild(boundBoxBTrans);

	boundBoxATrans->addChild(genCube);
	boundBoxBTrans->addChild(genCube);
}
void Scene::initializeObjects()
{
	/* add all the objects to the vector */
	collidableObjects.push_back(player);
	collidableObjects.push_back(aI);
}
bool Scene::isCollide()
{
	int size = collidableObjects.size();
	for (int i = 0; i < size; i++)
	{
		collidableObjects[i]->getCollided = false;
		for (int j = i + 1; j < size; j++)
		{
			if (collidableObjects[i]->collidesWith(collidableObjects[j]))
			{
				collidableObjects[i]->getCollided = true;
				if (collidableObjects[i]->movable) {
					collidableObjects[i]->handleCollision(collidableObjects[j]);

				}
				
				if (collidableObjects[j]->movable) {
					collidableObjects[j]->handleCollision(collidableObjects[i]);

				}
				//collidableObjects[j]->handleCollision(collidableObjects[i]);
			}
		}
	}
	return player->collidesWith(aI);
}
void Scene::zoom(float scrollOffset, glm::vec3 & cam_pos)
{
	float fact = 0.0f;
	if (scrollOffset > 0)
		fact = 1.5f;
	if (scrollOffset < 0)
		fact = 0.75f;
}
void Scene::jumpPlayer(bool accel)
{
	player->jump(accel);
}
Scene::~Scene()
{
	delete(skyBox);
	delete(worldGroup);	
}