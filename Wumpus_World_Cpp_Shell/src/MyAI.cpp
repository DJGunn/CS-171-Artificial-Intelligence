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
	mapState.currentRoom = NULL; //null out current room from our mapstate

	//populate our initial map of rooms
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{

			rooms[i][j] = room(i, j);
		}
	}

	//for the sake of our initial AI pre-load the command queue to step out then come back and climb
	/*myActionQueue.push(FORWARD);
	myActionQueue.push(TURN_LEFT);
	myActionQueue.push(TURN_LEFT);
	myActionQueue.push(FORWARD);
	myActionQueue.push(CLIMB);*/
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
		mapState.agentHasGold = true; //record that we got the gold

		return GRAB; //pick up the gold
	}
	//ensuring we catch wumpus death is second most priority
	if (scream) mapState.wumpusIsDead = true;
	//catching the dimensions of the map is third most priority
	if (bump && mapState.myAgent.myFacing == EAST && !mapState.mapWidthKnown) { mapState.mapWidth = mapState.myAgent.xPos+1; mapState.mapWidthKnown = true; }
	if (bump && mapState.myAgent.myFacing == NORTH && !mapState.mapHeightKnown) { mapState.mapHeight = mapState.myAgent.yPos+1; mapState.mapHeightKnown = true; }

	//Our proper loop starts here
	if (!myActionQueue.empty()) { //First things first is perform the actions in our queue
		chosenAction = myActionQueue.front();
		myActionQueue.pop();

		return chosenAction;
	}
	else
	{
		//update our map//
		//update current room
		updateRoom(rooms[mapState.myAgent.xPos][mapState.myAgent.yPos]);

		//update each room next to us
		// TODO: make sure map size is able to grow
		if (0 < mapState.myAgent.xPos) updateRoom(rooms[mapState.myAgent.xPos - 1][mapState.myAgent.yPos]);
		if (mapState.myAgent.xPos < mapState.mapWidth-1) updateRoom(rooms[mapState.myAgent.xPos+1][mapState.myAgent.yPos]);


		//choose an action//

		//execute first action in queue//
	}



	//when goal is adjacent (a child) use frontier priority queue
	//when goal is not adjacent use explored queue with target as explored node adjacent to target, then use frontier priority queue again
	//if find gold, use A* on explored queue to find way home since we only want to backtrack















	/////////////////////////////////////////


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
	if (!myActionQueue.empty()) {
		chosenAction = myActionQueue.front();
		myActionQueue.pop();

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
	if (p_rooms == NULL) p_rooms = mapState.currentRoom = tempRoom; //add tempRoom as first node and current room
	else {
		switch (mapState.myAgent.myFacing)
		{
		case NORTH:
			tempRoom->p_south = mapState.currentRoom; //set link in new room back to this room
			mapState.currentRoom->p_north = tempRoom; //set link in this room to new room
			break;
		case EAST:
			tempRoom->p_west = mapState.currentRoom;
			mapState.currentRoom->p_east = tempRoom;
			break;
		case SOUTH:
			tempRoom->p_north = mapState.currentRoom;
			mapState.currentRoom->p_south = tempRoom;
			break;
		case WEST:
			tempRoom->p_east = mapState.currentRoom;
			mapState.currentRoom->p_west = tempRoom;
			break;
		default:
			cout << "Error: No case chosen when adding room to node network." << endl;
			break;
		}
	}

}

void MyAI::updateRoom(room &myRoom)
{
	//If this is our first time being in this room, update info, if it's already been explored we don't need to update it
	if (mapState.myAgent.xPos == myRoom.xLoc && mapState.myAgent.yPos == myRoom.yLoc && myRoom.explored == false)
	{
		//percepts
		myRoom.stench = percepts[STENCH];
		myRoom.breeze = percepts[BREEZE];
		myRoom.glitter = percepts[GLITTER];
		myRoom.bump = percepts[BUMP];
		myRoom.scream = percepts[SCREAM];

		//update this room status
		myRoom.safe = myRoom.explored = true;
		myRoom.pit = myRoom.maybePit = false;
		myRoom.wumpus = myRoom.maybeWumpus = false;

		//make updates based on this room's percepts
		if (myRoom.stench); //update adjacent rooms which are not marked safe
		if (myRoom.breeze); //update adjacent rooms which are not marked safe
		if (myRoom.glitter) myRoom.goldRoom = true; //we need to pick up the gold and return to start and climb out
		//BUMP & SCREAM are handled in the main loop

		//if we just happen to be in row or column 7, update map width/height known if itisn't already to avoid bumps
		if (mapState.myAgent.xPos + 1 == mapState.mapMaxDimensions && !mapState.mapWidthKnown) mapState.mapWidthKnown = true;
		if (mapState.myAgent.yPos + 1 == mapState.mapMaxDimensions && !mapState.mapHeightKnown) mapState.mapHeightKnown = true;
	}
	//if this room isn't the room the agent is in, then it's adjacent, update appropriately
	else
	{
		//need logic to determine if pit/wumpus

		//logic to determine if clear

		//if this room is adjacent to a room with no stench/breeze, then this room is also safe
		if (!percepts[STENCH] && !percepts[BREEZE])
		{
			//update room status
			myRoom.safe = true;
			myRoom.pit = myRoom.maybePit = false;
			myRoom.wumpus = myRoom.maybeWumpus = false;
		}
	}
		
	

}

/**
NOTES:
- Will need to make it so that nodes get added to (priority)queues for A*
- Update addRoom to have the rest of the percepts in it



**/