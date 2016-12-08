#include "ParticleMaster.h"

using namespace std;


bool ParticleMaster::addChild(Particle * child)
{
	if (child != NULL)
	{
		childrens.push_back(child);
		return true;
	}
	else
		return false;
}
/* Try to figure out a solution for the case that two pointers are pointing to the same object */
bool ParticleMaster::removeChild(Particle * child)
{
	if (child != NULL)
	{
		//childrens.remove(child);
		return true;
	}
	else
		return false;
}

void ParticleMaster::update()
{
	/** Call draw on the childeren */

	printf("Begin Particle loopping: \n");
	int i = 0;
	for (int i = 0; i < childrens.size(); i ++)
	{
		printf(" Particle %d about to update\n", i);
		printf(" Particle update: (old pos = (%f, %f, %f)\n", childrens[i]->pos.x, childrens[i]->pos.y, childrens[i]->pos.z);
		printf(" Particle update: (v=%f, r_time=%t, l=%t)\n", childrens[i]->velocity.y, (float)childrens[i]->elapsedTime, (float)childrens[i]->lifeLength);
		childrens[i]->update();
		if(!childrens[i]->isLive()) {
			printf("  Particle %d died out\n", i);
			childrens.erase(childrens.begin() + i);
		}
	}
}


void ParticleMaster::draw(glm::mat4 cMatrix)
{
	/** Call draw on the childeren */

	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->draw(cMatrix);
	}
}
