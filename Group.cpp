#include "Group.h"

using namespace std;


bool Group::addChild(Node * child)
{
	if (child != NULL)
	{
		children.push_back(child);
		return true;
	}
	else
		return false;
}
/* Try to figure out a solution for the case that two pointers are pointing to the same object */
bool Group::removeChild(Node * child)
{
	if (child != NULL)
	{
		bool remove = false;
		for (int i = 0; i < children.size(); i++) {
			if (children[i] == child) {
				children.erase(children.begin() + i);
				return true;
			}
		}
	}
	return false;
}
void Group::update()
{
	/** Call draw on the childeren */
	
	for(int i = 0; i < children.size(); i++){
		children[i]->update();
	}
}

void Group::draw(glm::mat4 cMatrix)
{	
	for (int i = 0; i < children.size(); i++) {
		children[i]->draw(cMatrix);
	}
}

Group::~Group() {
	for each(Node * node in children) {
		//delete(node);
	}
}