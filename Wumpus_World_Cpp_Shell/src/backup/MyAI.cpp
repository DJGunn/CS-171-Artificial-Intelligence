// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"


MyAI::MyAI() : Agent()
{
	firstTurn = true; //initialize our first turn flag
	p_rooms = NULL; //null out p_rooms
	currentMapState.currentRoom = NULL; //null out current room from our mapstate

	//for the sake of our initial AI pre-load the command queue to step out then come back and climb
	babyAIQueue.push(FORWARD);
	babyAIQueue.push(TURN_LEFT);
	babyAIQueue.push(TURN_LEFT);
	babyAIQueue.push(FORWARD);
	babyAIQueue.push(CLIMB);
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	Action chosenAction = CLIMB; //what we return, default to CLIMB
	percepts = { stench, breeze, glitter, bump, scream };

	//Picking up gold is always our first priority
	if (glitter) {
		currentMapState.agentHasGold = true; //record that we got the gold

		return GRAB; //pick up the gold
	}

	//on our first turn we need to make our base room
	if (firstTurn)
	{
		firstTurn != firstTurn; //toggle flag

		//make our first room
		addRoom(percepts);

		if (stench || breeze) { return chosenAction; /*climb out immmediately if there's any danger in square 0,0 on our first turn*/ }
	}

	//on subsequent turns we run as normal


	//moves us forward once then comes back
	if (!babyAIQueue.empty()) {
		chosenAction = babyAIQueue.front();
		babyAIQueue.pop();

		return chosenAction;
	}

	return chosenAction; //if we reach this state there's been an error
}

//perform A* search and return the most optimal way to reach our target destination
std::queue<Action> MyAI::findPath(int xDest, int yDest)
{
	std::queue<Action> returnPath;

	return returnPath;
}

//instantiate a new room and add it to our map
void MyAI::addRoom(bool percepts[])
{
	//create room
	room * tempRoom = new room;

	//populate room with percepts
	tempRoom->stench = percepts[STENCH];
	tempRoom->breeze = percepts[BREEZE];
	tempRoom->glitter = percepts[GLITTER];
	tempRoom->bump = percepts[BUMP];
	tempRoom->scream = percepts[SCREAM];

	//null the pointers
	tempRoom->p_north = NULL;
	tempRoom->p_east = NULL;
	tempRoom->p_south = NULL;
	tempRoom->p_west = NULL;

	//add room to list
	if (p_rooms == NULL) p_rooms = currentMapState.currentRoom = tempRoom; //add tempRoom as first node and current room
	else {
		switch (currentMapState.myAgent.myFacing)
		{
		case NORTH:
			tempRoom->p_south = currentMapState.currentRoom; //set link in new room back to this room
			currentMapState.currentRoom->p_north = tempRoom; //set link in this room to new room
			break;
		case EAST:
			tempRoom->p_west = currentMapState.currentRoom;
			currentMapState.currentRoom->p_east = tempRoom;
			break;
		case SOUTH:
			tempRoom->p_north = currentMapState.currentRoom;
			currentMapState.currentRoom->p_south = tempRoom;
			break;
		case WEST:
			tempRoom->p_east = currentMapState.currentRoom;
			currentMapState.currentRoom->p_west = tempRoom;
			break;
		default:
			cout << "Error: No case chosen when adding room to node network." << endl;
			break;
		}
	}

}

/**
NOTES:
- Will need to make it so that nodes get added to (priority)queues for A*
- Update addRoom to have the rest of the percepts in it



**/