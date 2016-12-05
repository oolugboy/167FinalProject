#include "BuildingGrammar.h"

using namespace std;

/*------Shape Table w/ Probabilities
	Shape Name			Chance

-Bottom:
	Box					50%
	Cylinder			50%

-Middle:
	Box					100%

-Top:
	Box Roof			50%
	Pyramid				25%
	Trapezoid Prism		25%
*/

/*------Symbol Table
	Symbol Name			Description

-Replaceable Symbols:
	B					Bottom Replaceable
	M					Middle Replaceable
	T					Top Replaceable

-Non-replaceable Symbols: (these represent the shapes)
	0					Box Bottom
	1					Cylinder Bottom

	2					Box Middle

	3					Box Top
	4					Pyramid Top
	5					Trapezoid Top

*/

/*------Rule Table w/ Probabilities
	Rule				Chance

	B -> 0M				50%
	B -> 1M				50%

	M -> 2M				80%
	M -> 2T				20%

	T -> 3				50%
	T -> 4				25%
	T -> 5				25%
*/

//Chances for each rule

//Set 1 : Symbol B
const int CHANCE_0M = 50;
const int CHANCE_1M = 50;
//Set 2 : Symbol M
const int CHANCE_2M = 80;
const int CHANCE_2T = 20;
//Set 3 : Symbol T
const int CHANCE_3 = 50;
const int CHANCE_4 = 25;
const int CHANCE_5 = 25;

int choice = 0;		//Current choice given by the RNG
string gram_str;	//Grammar String

Sphere * sphere;
Cube * cube;

//Constructor
BuildingGrammar::BuildingGrammar()
{
	//Seed our RNG using current time (this will give unique results for each die roll)
	srand(time(NULL));

	//Initialize Our building shapes
	//WARNING NEED TO MOVE THIS ELSEWHERE SO WE DON'T RUN OUT OF MEMORY
	sphere = new Sphere(2, false);
	cube = new Cube(false);
}

//Deconstructor
BuildingGrammar::~BuildingGrammar()
{
}

/*
* BuildGrammar
* returns MatrixTransform *
*/
MatrixTransform * BuildingGrammar::Build(glm::vec3 position, glm::vec3 scale, float rotationAngle)
{

	glm::vec3 pos = position;
	glm::vec3 scl = scale;
	float rotAngle = rotationAngle;

	pos.y = scl.y;

	//------Build Grammar String that has no more paths

	//Start with a replaceable bottom
	gram_str = "B";

	//Parse the string to replace any replaceable symbols using our rules
	bool finished = false;

	while(finished == false)
	{
		finished = ParseString();
	}

	//DEBUG Print the final string
	cout << "Final String: " << gram_str << endl;

	//------Build building using the string

	//Create the Group (or coordinate space) for this building
	//and initialie size and position
	MatrixTransform * buildingTrans = new MatrixTransform();
	buildingTrans->transformMatrix = buildingTrans->transformMatrix * glm::translate(glm::mat4(1.0f), pos);
	buildingTrans->transformMatrix = buildingTrans->transformMatrix * glm::scale(glm::mat4(1.0f), scl);
	buildingTrans->transformMatrix = buildingTrans->transformMatrix * glm::rotate(glm::mat4(1.0f), rotAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));

	float height = 0;
	
	//Parse the string and add the shapes to the building
	for (int i = 0; i < gram_str.length(); i++) 
	{
		char cur_symbol = gram_str[i];
		switch (cur_symbol)
		{
			case '0':
			{
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(cube);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Cube" << endl;
				break;
			}
			case '1':
			{	
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(sphere);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Cube" << endl;
				break;
			}
			case '2':
			{
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(sphere);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Sphere" << endl;
				break;
			}
			case '3':
			{
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(cube);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Sphere" << endl;
				break;
			}
			case '4':
			{
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(sphere);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Sphere" << endl;
				break;
			}
			case '5':
			{
				MatrixTransform * shapeTrans = new MatrixTransform();
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				shapeTrans->transformMatrix = shapeTrans->transformMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, height, 0.0f));
				shapeTrans->addChild(cube);
				buildingTrans->addChild(shapeTrans);
				cout << "Building Sphere" << endl;
				break;
			}
			default:
				break;
		}
		height += 2.0f;
	}

	//Return the final building
	return buildingTrans;
}

/*
 * ParseString
 * returns bool
*/

bool BuildingGrammar::ParseString() 
{
	//DEBUG
	cout << "Grammar String " << gram_str << endl;

	string temp_str = "";	//Temp string to build during symbol replacement
	bool finished = true;	//Whether or not we have fully finished replacing symbols

	//Parse the current grammar string
	//For each symbol we will use a rule to replace that symbol with new symbol(s) (using our rule table)
	for (int i = 0; i < gram_str.length(); i++)
	{
		char cur_symbol = gram_str[i];	//Get current symbol
		choice = rand() % 100 + 1;	//Get a random number from 1-100 (simulate % chance)
		
		//DEBUG
		cout << cur_symbol << endl;

		//Switch case for which rule to use depending on the symbol
		switch(cur_symbol) {
			case 'B':	//Symbol B
			{
				finished = false;
				//Rule 1: B -> 0M (50%)
				if (choice - CHANCE_0M <= 0)
					temp_str.append("0M");
				//Rule 2: B -> 1M (50%)
				else if (choice - (CHANCE_0M + CHANCE_1M) <= 0)
					temp_str.append("1M");
				break;
			}
			case 'M':	//Symbol M
			{
				finished = false;
				//Rule 3: M -> 2M (80%)
				if (choice - CHANCE_2M <= 0 )
					temp_str.append("2M");
				//Rule 4: M -> 2T (20%)
				else if (choice - (CHANCE_2M + CHANCE_2T) <= 0 )
					temp_str.append("2T");
				break;
			}
			case 'T':	//Symbol T
			{
				finished = false;
				//Rule 5: T -> 3 (50%)
				if (choice - CHANCE_3 <= 0)
					temp_str.append("3");
				//Rule 6: T -> 4 (25%)
				else if (choice - (CHANCE_3 + CHANCE_4) <= 0)
					temp_str.append("4");
				//Rule 7: T -> 5 (25%)
				else if (choice - (CHANCE_3 + CHANCE_4 + CHANCE_5) <= 0)
					temp_str.append("5");
				break;
			}
			default:	//All other symbols (no rules for these, just append them back onto the new string)
			{
				temp_str += cur_symbol;
				break;
			}
		}
	}

	//Set our grammar string as the new generated string
	gram_str = temp_str;

	//Return whether or not we have finished replacing all replaceable symbols in the string
	return finished;
}