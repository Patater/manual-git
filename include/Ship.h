/*
 *  Ship.h
 *  
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#ifndef SHIP_H
#define SHIP_H

#include <nds.h>

#include <math.h>

#include "Coordinate.h"

#define PI (3.14159265358979323846264338327)

using namespace std;


class Ship {
	
private:
	int shipHeight;
	int shipWidth;
	
	//used for ship kinematics
	Coordinate position;
	Coordinate velocity;
	float angle; //in radians
	
	//motion properties of the ship
	float turnSpeed; //in radians (should be a relatively small portion of 2¹)
	float thrust;
	float maxSpeed;
	float mass;
	
	//internal conversion routines
	float deg512ToRadians(int deg512);
	
	void init(const Ship & other);
	
public:
		
	Ship(int _shipWidth, int _shipHeight);		//default constructor
	Ship(const Ship & other);				//copy constructor
	virtual ~Ship();							//deconstructor
	
	Ship operator=(const Ship & other);	//= operator
	
	void accelerate();
	
	void moveShip();
	
	void reverseTurn();
	
	void turnClockwise();
	
	void turnCounterClockwise();
	
	Coordinate getPosition();
	
	float getAngleRad();
	
	int getAngleDeg512();
	
};

#endif
