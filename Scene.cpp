#include "Scene.h"

using namespace std;


float PI = 3.14159265;
float modder = pow(10, 9) + 7;
float defaultAccel = 0.01f;

/*~~ SHAPE GRAMMAR TESTING*/
MatrixTransform * buildingTrans;
/*~~ END */
vector <const GLchar * > playSkyFaces;
vector <const GLchar * > aISkyFaces;

/*AUDIO*/
ISoundEngine *SoundEngine = createIrrKlangDevice();
/*END*/


Scene::Scene(int numRobots, GLint shaderProgram1, GLint shaderProgram2)
{
	playSkyFaces.resize(6);
	playSkyFaces[0] = "textures/playerSkyBox/perdicus_rt.ppm";
	playSkyFaces[1] = "textures/playerSkyBox/perdicus_lf.ppm";
	playSkyFaces[2] = "textures/playerSkyBox/perdicus_up.ppm";
	playSkyFaces[3] = "textures/playerSkyBox/perdicus_dn.ppm";
	playSkyFaces[4] = "textures/playerSkyBox/perdicus_bk.ppm";
	playSkyFaces[5] = "textures/playerSkyBox/perdicus_ft.ppm";

	Ball::playerTexID = SkyBox::loadCubeMap(playSkyFaces);

	aISkyFaces.resize(6);
	aISkyFaces[0] = "textures/aISkyBox/hell_rt.ppm";
	aISkyFaces[1] = "textures/aISkyBox/hell_lf.ppm";
	aISkyFaces[2] = "textures/aISkyBox/hell_up.ppm";
	aISkyFaces[3] = "textures/aISkyBox/hell_dn.ppm";
	aISkyFaces[4] = "textures/aISkyBox/hell_bk.ppm";
	aISkyFaces[5] = "textures/aISkyBox/hell_ft.ppm";

	/* Load the textures for the players*/ 
	Ball::aITexID = SkyBox::loadCubeMap(aISkyFaces);

	t = clock();
	m_shaderProgram1 = shaderProgram1;
	m_shaderProgram2 = shaderProgram2;
	skyBox = new SkyBox();
	worldGroup = new Group();

	/* The general relevant geometries */
	genSphere = new Sphere(1.0f, false);
	genCube = new Cube(true);
	numAgents = 20;
	randomInitial(1);
}

void Scene::randomInitial(int seed) {
	/* Initialize the required variables */
	worldMatTrans = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	//worldGroup->children.clear();

	int world_grids = 100;
	city = new City(world_grids);
	BuildingGrammar * buildingGram = new BuildingGrammar();

	if (seed >= 0) { srand(0); }
	else { srand(time(NULL)); }

	/* Set the initial position for the player */
	playerBallTrans = new MatrixTransform();
	player = new Ball(true, glm::vec3(0.0f, 1, 0.0f), playerBallTrans);
	collidableObjects.push_back(player);
	city->addObject(collidableObjects[0]->matrixT->transformMatrix);
	collidableObjects[0]->matrixT->addChild(genCube);
	collidableObjects[0]->matrixT->addChild(genSphere);
	worldGroup->addChild(collidableObjects[0]);
	
	Window::camera->player = player;
	
	/** random position for the aI's */
	for (int i = 1; i < numAgents; i++)
	{
		float xpos = ((rand() % 1000) / 500.0f - 1.0f) * (world_grids / 2) * 0.75f;
		float zpos = ((rand() % 1000) / 500.0f - 1.0f) * (world_grids / 2) * 0.75f;
		glm::vec3 trans = glm::vec3(xpos, 1.0f, zpos);
		collidableObjects.push_back(new Ball(false, trans, new MatrixTransform()));	
		city->addObject(collidableObjects[i]->matrixT->transformMatrix);
		collidableObjects[i]->matrixT->addChild(genCube);
		collidableObjects[i]->matrixT->addChild(genSphere);		
		worldGroup->addChild(collidableObjects[i]);
	}

	initializeObjects();

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
			//TODO: buildings are too height, cannot see anything for debug, might need a max hieght, and make the height depend on scale?
			buildingTrans = buildingGram->Build(glm::vec3(xpos, 0.0f, zpos), glm::vec3(size), rotAngle);
			worldGroup->addChild(buildingTrans);
			/*MatrixTransform* cube1 = new MatrixTransform();
			glm::mat4 cube1_matrix = cube1->transformMatrix * translate * scale * rot;
			cube1->transformMatrix = cube1_matrix;
			cube1->addChild(genCube);
			worldGroup->addChild(cube1);*/
		}
	}
	worldGroup->addChild(city);
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
	//worldGroup->draw(worldMatTrans);
	worldGroup->draw(glm::mat4(1.0f));


	/* Test the collision detection */
	if (isCollide())
		cout << " Collision detected " << endl;
}
void Scene::update()
{
	worldGroup->update();
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
void Scene::acceleratePlayers(bool posAccel)
{
	player->accelerate(posAccel);
	for (int i = 1; i < numAgents; i++)
	{
		bool aIAccel = (rand() % 2);
		if (collidableObjects[i]->movable == true)
		{
			collidableObjects[i]->accelerate(aIAccel);
			collidableObjects[i]->turn = rand() % 3;
		}
	}	
}

void Scene::initializeObjects()
{	

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
				if(collidableObjects[i] == player || collidableObjects[j] == player)
					SoundEngine->play2D("audio/bounce.wav", GL_FALSE);
			}
		}
	}	
	return false;
}
void Scene::zoom(float scrollOffset, glm::vec3 & cam_pos)
{
	float fact = 0.0f;
	if (scrollOffset > 0){
		fact = 1.5f;
	}
	if (scrollOffset < 0) {
		fact = 0.75f;
	}

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