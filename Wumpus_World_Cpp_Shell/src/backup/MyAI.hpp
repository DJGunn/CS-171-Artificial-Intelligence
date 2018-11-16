// ======================================================================
// FILE:        MyAI.hpp
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

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <queue>
#include <iostream>

class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);

	//perform A* search and return most optimal route to destination
	std::queue<Action> findPath(int xDest, int yDest);

	//instantiate a new room and add it to our map
	void addRoom(bool percepts[]);
	
private:
	enum Direction { NORTH, EAST, SOUTH, WEST };
	enum Percepts {STENCH, BREEZE, GLITTER, BUMP, SCREAM};

	struct room {
		bool stench = false, breeze = false, glitter = false, bump = false, scream = false;
		bool safe = false, pit = false, maybePit = false, wumpus = false, maybeWumpus = false, goldRoom = false;

		room * p_north;
		room * p_east;
		room * p_south;
		room * p_west;

		int hValue, gValue;
	};

	struct agent {
		int xPos = 0, yPos = 0;
		Direction myFacing = EAST; //agent starts facing right
	};

	struct {
		bool wumpusIsDead = false, agentHasGold = false, agentHasArrow = true;
		int mapX = 4, mapY = 4; //map can be minimum of 4x4
		const int mapMaxDimensions = 7; //map can be maximum of 7x7

		room * currentRoom;
		agent myAgent; //our agent's information
	} currentMapState; //this will hold our map state in the current turn

	std::queue<Action> babyAIQueue;

	bool firstTurn;
	room * p_rooms; //variable to hold our room network
	bool percepts[5]; //our percepts that we receive each round
};

#endif