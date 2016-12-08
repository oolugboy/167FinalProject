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
		children.remove(child);
		return true;
	}
	else
		return false;
}
void Group::update()
{
	/** Call draw on the childeren */
	list< Node *>::const_iterator lit = children.begin();
	list< Node *>::const_iterator len = children.end();

	for (; lit != len; lit++)
	{
		Node * curr = (*lit);
		curr->update();
	}
}

void Group::draw(glm::mat4 cMatrix)
{	
	/** Call draw on the childeren */
	list< Node *>::const_iterator lit = children.begin();
	list< Node *>::const_iterator len = children.end();

	for (; lit != len && !children.empty(); lit++)
	{
		//cout << " about to call the child to draw " << endl;
		Node * curr = (*lit);
		curr->draw(cMatrix);
	}
}