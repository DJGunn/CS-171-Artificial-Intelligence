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
#include <stack>
#include <map>
#include <cmath>
#include <iostream>

class MyAI : public Agent
{
	enum Direction { NORTH, EAST, SOUTH, WEST };
	enum Percepts { STENCH, BREEZE, GLITTER, BUMP, SCREAM };

	struct room {
		//this room's percepts
		bool stench = false, breeze = false, glitter = false, bump = false, scream = false;

		//this room's status
		bool safe = false, goldRoom = false, explored = false;

		int fValue = 1, gValue = 1, hValue = 0, xLoc, yLoc;

		double pWumpus = 0; //probability wumpus is in this room
		double pPitfall = 0; //probability pitfall is in this room

		Action nextAction;
	};

	struct CompareRoom {
		bool operator()(room const& r1, room const& r2)
		{
			return r1.fValue > r2.fValue;
		}
	};

	struct agent {
		int xPos = 0, yPos = 0;
		bool hasGold = false, hasArrow = true;
		Direction facing = EAST; //agent starts facing right
	};

	struct {
		bool wumpusIsDead = false;
		int wumpusTracks = 0;
		int mapWidth = 7, mapHeight = 7; //map can be minimum of 4x4, but we assume 7 and shrink on bump
		const int mapMaxDimensions = 7; //map can be maximum of 7x7

		bool mapWidthKnown = false, mapHeightKnown = false;
	} mapState; //this will hold our map state in the current turn

public:
	MyAI(void);

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

	void updateRoom(room &myRoom);

	void updateMapHFunction();

private:
	agent myAgent; //our agent's information
	room rooms[7][7]; //array to hold our room network

	std::queue<Action> myActionQueue;
	std::priority_queue<room, std::vector<room>, CompareRoom> frontierQueue;
	std::map<std::pair<int,int>, room> exploredMap;
	std::stack<Action> myExitActionStack;

	bool firstTurn;
	bool percepts[5]; //our percepts that we receive each round
	Action chosenAction; //what we return as our action each turn, this is defaulted to CLIMB in the constructor
};

#endif