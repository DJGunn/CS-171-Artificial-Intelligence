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

	//populate our initial map of rooms
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{

			rooms[i][j] = room(i, j);
		}
	}

	//set our first chosen action
	chosenAction = CLIMB;

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

	//save our percepts
	percepts[0] = stench;
	percepts[1] = breeze;
	percepts[2] = glitter;
	percepts[3] = bump;
	percepts[4] = scream;

	//Picking up gold is always our first priority
	if (glitter && !myAgent.hasGold) {
		myAgent.hasGold = true; //record that we got the gold

		return GRAB; //pick up the gold
	}
	//If we have gold, getting out becomes our top priority
	if (myAgent.hasGold)
	{
		//TODO: Get out of the cave using backtrack stack of actions/rotations
	}
	//ensuring we catch wumpus death is third most priority
	if (scream) mapState.wumpusIsDead = true;
	//catching the dimensions of the map is fourth most priority
	if (bump && myAgent.facing == EAST && !mapState.mapWidthKnown) { mapState.mapWidth = myAgent.xPos+1; mapState.mapWidthKnown = true; }
	if (bump && myAgent.facing == NORTH && !mapState.mapHeightKnown) { mapState.mapHeight = myAgent.yPos+1; mapState.mapHeightKnown = true; }

	//Our proper loop starts here
	if (!myActionQueue.empty()) { //First things first is perform the actions in our queue
		chosenAction = myActionQueue.front();
		myActionQueue.pop();

		return chosenAction;
	}
	else
	{
		//If we are here it means that we've just finished movement maybe; check chosenAction, which still holds our last action
		//update our map//
		if (chosenAction == FORWARD && rooms[myAgent.xPos][myAgent.yPos].explored == false) //if our last action was moving into a new square, and this square is previously unexplored
		{
			// TODO: remove this room from our Frontier rooms
			updateRoom(rooms[myAgent.xPos][myAgent.yPos]); //update current room + adjacent rooms (handled in the update room function automatically)
			// TODO: add this room to our Explored rooms
		}




		//choose an action//
		// TODO: Choose an action
		// TODO: Save all rotate/move actions to stack so we can backtrack out when we have gold
		//execute first action in queue//
	}

	return chosenAction; //if we reach this state there's been an error
}

//perform A* search and return the most optimal way to reach our target destination
/*std::queue<Action> MyAI::findPath(int xDest, int yDest)
{
	std::queue<Action> returnPath;

	return returnPath;
}*/



void MyAI::updateRoom(room &thisRoom)
{
	//If this is our first time being in this room, update info, if it's already been explored we don't need to update it
	if (myAgent.xPos == thisRoom.xLoc && myAgent.yPos == thisRoom.yLoc && thisRoom.explored == false) //TODO: Figure out if thisRoom.explored is redundant, We should only be updating unexplored rooms
	{
		//percepts
		thisRoom.stench = percepts[STENCH];
		thisRoom.breeze = percepts[BREEZE];
		thisRoom.glitter = percepts[GLITTER];
		thisRoom.bump = percepts[BUMP];
		thisRoom.scream = percepts[SCREAM];

		//update this room status
		thisRoom.pPitfall = 0.0;
		thisRoom.pWumpus = 0.0;
		thisRoom.safe = thisRoom.explored = true;

		//make updates based on this room's percepts
		if (thisRoom.stench); //update adjacent rooms which are not marked safe - We can probably do this with a recursive call
		if (thisRoom.breeze); //update adjacent rooms which are not marked safe
		if (thisRoom.glitter) thisRoom.goldRoom = true; //we need to pick up the gold and return to start and climb out
		//if (thisRoom.bump); These are logical placeholders for readability
		//if (thisRoom.scream); Bump & Scream are handled in the main loop.

		//if we just happen to be in row or column 7, update map width/height known if itisn't already to avoid bumps
		//each of these will only ever trigger true once
		if (myAgent.xPos + 1 == mapState.mapMaxDimensions && !mapState.mapWidthKnown) mapState.mapWidthKnown = true;
		if (myAgent.yPos + 1 == mapState.mapMaxDimensions && !mapState.mapHeightKnown) mapState.mapHeightKnown = true;

		//update the four rooms around us
		if (thisRoom.xLoc - 1 > 0) updateRoom(rooms[thisRoom.xLoc - 1][thisRoom.yLoc]); //update left room
		if (thisRoom.xLoc + 1 < mapState.mapWidth) updateRoom(rooms[thisRoom.xLoc + 1][thisRoom.yLoc]); //update right room

		if (thisRoom.yLoc - 1 > 0) updateRoom(rooms[thisRoom.xLoc][thisRoom.yLoc - 1]); //update bottom room
		if (thisRoom.yLoc + 1 < mapState.mapHeight) updateRoom(rooms[thisRoom.xLoc][thisRoom.yLoc + 1]); //update right room
	}
	//if this room isn't the room the agent is in, then it's adjacent, update appropriately
	else if ((myAgent.xPos != thisRoom.xLoc || myAgent.yPos != thisRoom.yLoc) && thisRoom.explored == false)
	{
		//TODO: Account for walls in the probability calculations
		//TODO: when wumpus is found, remove all other probabilities of a wumpus from other rooms
		//need logic to determine if pit/wumpus
		if (percepts[STENCH] && !thisRoom.safe) {
			if (thisRoom.pWumpus == 0) thisRoom.pWumpus = 0.25;
			else if (thisRoom.pWumpus == 0.25) thisRoom.pWumpus = 0.5;
			else if (thisRoom.pWumpus == 0.5) thisRoom.pWumpus = 1.0;
		}
		if (percepts[BREEZE] && !thisRoom.safe)
		{
			if (thisRoom.pPitfall == 0) thisRoom.pPitfall = 0.25;
			else if (thisRoom.pPitfall == 0.25) thisRoom.pPitfall = 0.5;
			else if (thisRoom.pPitfall == 0.5) thisRoom.pPitfall = 0.75;
			else if (thisRoom.pPitfall == 0.75) thisRoom.pPitfall = 1.0;
		}

		//if this room is adjacent to a room with no stench/breeze, then this room is also safe
		if (!percepts[STENCH]) thisRoom.pWumpus = 0.0;
		if (!percepts[BREEZE]) thisRoom.pPitfall = 0.0;
		if (thisRoom.pWumpus == 0 && thisRoom.pPitfall == 0) thisRoom.safe = true;
	}
}

/**
NOTES:
- Will need to make it so that nodes get added to (priority)queues for A*
- Update addRoom to have the rest of the percepts in it

	//when goal is adjacent (a child) use frontier priority queue
	//when goal is not adjacent use explored queue with target as explored node adjacent to target, then use frontier priority queue again
	//if find gold, use A* on explored queue to find way home since we only want to backtrack


**/