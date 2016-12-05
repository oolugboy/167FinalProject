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

	srand(0);
	for (int i = 0; i < 10; i++) {
		printf("%f %f %f\n", (rand() % 10000) / 100.0f, (rand() % 10000) / 100.0f, (rand() % 10000) / 100.0f );
	}


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

	//player = new Ball(true, glm::vec3(0,0,0));	
	//player->sphere = genSphere;
	//playerTrans = glm::vec3(0.0f, 0.0f, 0.0f);

	//aI = new Ball(false, glm::vec3(0, 0, 0));
	//aI->sphere = genSphere;

	MatrixTransform* cube1 = new MatrixTransform();
	glm::mat4 cube1_matrix = cube1->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 4.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
	cube1->transformMatrix = cube1_matrix;
	cube1->addChild(genCube);

	MatrixTransform* cube2 = new MatrixTransform();
	glm::mat4 cube2_matrix = cube2->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, 0.0f, 2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	cube2->transformMatrix = cube2_matrix;
	cube2->addChild(genCube);


	city = new City(10);

	//worldGroup->addChild(cube1);
	//worldGroup->addChild(cube2);

	if (city->addObject(cube1_matrix)) {
		printf("add cube1 in city correctly \n");
		worldGroup->addChild(cube1);
	}


	if (city->addObject(cube2_matrix)) {
		printf("add cube2 in city correctly \n");
		worldGroup->addChild(cube2);
	}


	
	buildGraph();
	/* Initialize the sizes */
	initializeObjects();

	/*~~ SHAPE GRAMMAR TESTING*/
	BuildingGrammar * buildingGram = new BuildingGrammar();
	buildingTrans = buildingGram->Build(glm::vec3(3.0f, 0.0f, 0.0f));
	buildingTrans->transformMatrix = cube2_matrix;
	worldGroup->addChild(buildingTrans);
	/*~~ END */
	
}

void Scene::randomInitial(int seed) {
	if (seed >= 0) { srand(0); }
	else { srand(time(NULL)); }

	int world_grids = 100;
	city = new City(world_grids);
	for (int i = 0; i < world_grids; i++) {

	}
}
void Scene::draw()
{
	// Use the first shader program to draw the skybox
	glUseProgram(m_shaderProgram1);
	/* We first draw the skybox */
	skyBox->draw(m_shaderProgram1);	
	/** Now to use */
	glUseProgram(m_shaderProgram2);

	//Since skybox is fix size, so scale down the world to make the world looks bigger
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	worldGroup->draw(scale);
	
	//cout << " It took this " << t << " clicks to render the robots in this frame " << endl;
	worldGroup->update();

	if (Window::debug) {
		//TODO: draw cube boundary only if debug flag is on
		//city->drawCurves();
	}
}
void Scene::changePlayerDirection(float direction, bool posAccel)
{
	glm::mat4 rotMatrix;
	if (direction == 0)//left
	{
		//player->turn(true, false, posAccel);
		player->turn = 0;
	}
	if (direction == 2)//Right
	{
		//player->turn(false, true, posAccel);
		player->turn = 2;
	}	
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
	//player->accelerate(posAccel);
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
	//glm::vec3 diff = (player->initVelocity * time) + (0.50f * player->acceleration * pow(time, 2));
	//cout << " The diff is ";
	//player->printVector(diff);
	//playerTrans = playerTrans + diff;
	///*cout << " We are translating  ";
	//player->printVector(playerTrans); */
	//playerBallTrans->transformMatrix = glm::translate(glm::mat4(1.0f), playerTrans);
	//ballBTrans->transformMatrix = ballBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}
void Scene::buildGraph()
{
	/* Now to build the tree */	
	//worldGroup->addChild(playerBallTrans);	
	//worldGroup->addChild(ballBTrans);
	worldGroup->addChild(city);

	//playerBallTrans->addChild(player);
	//ballBTrans->addChild(aI);

	//playerBallTrans->addChild(boundBoxATrans);
	//ballBTrans->addChild(boundBoxBTrans);

	//boundBoxATrans->addChild(genCube);
	//boundBoxBTrans->addChild(genCube);
}
void Scene::initializeObjects()
{
/*	playerBallTrans->transformMatrix = playerBallTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, 0.0f, 0.0f));
	playerBallTrans->transformMatrix = playerBallTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));*/

	//ballBTrans->transformMatrix = ballBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -12.0f));	

	///* add all the objects to the vector */
	//collidableObjects.push_back(player);
	//collidableObjects.push_back(aI);
}
bool Scene::isCollide()
{
	//int size = collidableObjects.size();
	//for (int i = 0; i < size; i++)
	//{
	//	for (int j = i + 1; j < size; j++)
	//	{
	//		if (collidableObjects[i]->collidesWith(collidableObjects[j]))
	//		{
	//			collidableObjects[i]->handleCollision(collidableObjects[j]);
	//			collidableObjects[j]->handleCollision(collidableObjects[i]);
	//		}
	//	}
	//}
	//return player->collidesWith(aI);
	return false;
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
	//player->jump(accel);
}
Scene::~Scene()
{
	delete(skyBox);
	delete(worldGroup);	
}