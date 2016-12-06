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
	worldMatTrans = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	//for (int i = 0; i < 10; i++) {
	//	printf("%f %f %f\n", (rand() % 10000) / 100.0f, (rand() % 10000) / 100.0f, (rand() % 10000) / 100.0f );
	//}


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

	//aI = new Ball(false, glm::vec3(0, 0, -25.0f));
	//ballBTrans->transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -25.0f));
	//aI->sphere = genSphere;

	int world_grids = 100;
	city = new City(world_grids);

	BuildingGrammar * buildingGram = new BuildingGrammar();

	srand(1);
	for (int i = 0; i < world_grids; i++) {
		float xpos = ((rand() % 1000) / 500.0f - 1.0f) * (world_grids / 2) * 0.75f;
		float zpos = ((rand() % 1000) / 500.0f - 1.0f) * (world_grids / 2) * 0.75f;
		float size = ((rand() % 1000) / 1000.0f) * (0.2f * world_grids / 2 - 1.0f) + 1.0f;
		float rotAngle = (rand() % 1000) / 1000.0f * 90.0f;
		//printf("Add random building %d: (%f,%f), %f, %f\n", i, xpos, zpos, size, rotAngle);

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, 0.0f, zpos));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 rot = glm::mat4(1.0f);

		glm::mat4 cMatrix = translate * scale * rot;

		if (city->addObject(cMatrix)) {
			buildingTrans = buildingGram->Build(glm::vec3(xpos, 0.0f, zpos), glm::vec3(size), rotAngle);
			worldGroup->addChild(buildingTrans);
			//MatrixTransform* cube1 = new MatrixTransform();
			//glm::mat4 cube1_matrix = cube1->transformMatrix * translate * scale * rot;
			//cube1->transformMatrix = cube1_matrix;
			//cube1->addChild(genCube);
			//worldGroup->addChild(cube1);
		}
	}


	/*~~ SHAPE GRAMMAR TESTING*/
	//BuildingGrammar * buildingGram = new BuildingGrammar();
	//buildingTrans = buildingGram->Build(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 45.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 20.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 80.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 30.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(2.0f, 0.0f, 5.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f);
	//worldGroup->addChild(buildingTrans);
	//buildingTrans = buildingGram->Build(glm::vec3(-2.0f, 0.0f, 5.0f), glm::vec3(0.5f, 0.5f, 0.5f), 68.0f);
	//worldGroup->addChild(buildingTrans);
	/*~~ END */

	buildGraph();
	/* Initialize the sizes */
	initializeObjects();

	
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
	worldGroup->draw(worldMatTrans);
	

	/* Test the collision detection */
	if (isCollide())
		cout << " Collision detected " << endl;

	//cout << " It took this " << t << " clicks to render the robots in this frame " << endl;
	worldGroup->update();

	if (Window::debug) {
		//TODO: draw cube boundary only if debug flag is on
		//city->drawCurves();
	}
}
void Scene::changePlayerDirection(float direction, bool posAccel)
{
	//player->turn = direction;		
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
	//aI->accelerate(false);
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
	//glm::vec3 diff = glm::vec3(0.0f, 0.0f, 0.0f);
	//diff = (player->initVelocity * time) + (0.50f * player->acceleration * pow(time, 2));
	//cout << " The diff is ";
	//player->printVector(diff);	
	//playerBallTrans->transformMatrix = playerBallTrans->transformMatrix * glm::translate(glm::mat4(1.0f), diff);
	//
	///* For the aI part */
	//diff = (aI->initVelocity * time) + (0.50f * aI->acceleration * pow(time, 2));
	//cout << " The aI acceleration is ";
	//player->printVector(aI->acceleration);
	//ballBTrans->transformMatrix = ballBTrans->transformMatrix * glm::translate(glm::mat4(1.0f), diff); 
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
	/* add all the objects to the vector */
	//collidableObjects.push_back(player);
	//collidableObjects.push_back(aI);
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
	//return player->collidesWith(aI);
	return false;
}
void Scene::zoom(float scrollOffset, glm::vec3 & cam_pos)
{
	float fact = 0.0f;
	if (scrollOffset > 0) {
		fact = 1.5f;
		worldMatTrans = glm::scale(glm::mat4(1.0f), glm::vec3(fact)) * worldMatTrans;
	}
	if (scrollOffset < 0) {
		fact = 0.75f;
		worldMatTrans = glm::scale(glm::mat4(1.0f), glm::vec3(fact)) * worldMatTrans;
	}
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