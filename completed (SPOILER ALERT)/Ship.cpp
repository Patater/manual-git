/*
 *  Ship.cpp
 *  
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include "Ship.h"

#include <math.h>

int Ship::radToDeg512(float rad) {
	//unoptimized
	//return rad * (256/PI);
	
	//optimized
	return ((radians * 163) >> 1);
	
	//super optimized an more innacurate
}

void Ship::init(const Ship & other) {
	shipWidth = other.shipWidth;
	shipHeight = other.shipHeight;
	
	position.x = other.position.x;
	position.y = other.position.y;
	
	velocity.x = other.position.x;
	velocity.y = other.velocity.y;
	
	turnSpeed = other.turnSpeed;
	thrust = other.thrust;
	maxSpeed = other.maxSpeed;
	
	angle = other.angle;
	
	mass = other.mass;
}

Ship::Ship(int _shipWidth, int _shipHeight) {
	shipWidth = _shipWidth;
	shipHeight = _shipHeight;
	
	position.x = SCREEN_WIDTH/2 - shipWidth;
	position.y = SCREEN_HEIGHT/2 - shipHeight;
	
	velocity.x = 0;
	velocity.y = 0;
	
	turnSpeed = .0368;
	thrust = .05;
	maxSpeed = 1.0;
	
	angle = 0;
	
	mass = 1.0;
}

Ship::Ship(const Ship & other) {
	init(other);
}

Ship::~Ship() {
	//nothing
}

Ship Ship::operator=(const Ship & other) {
	init(other);
	
	return *this;
}

void Ship::accelerate() {
	
	float incX = thrust * sin(angle);
	float incY = -(thrust * cos(angle));
	
	velocity.x += incX;
	
	//the following method of speed limitation is not accurate, traveling diagonally is faster than straight, which is not the desired limitation
	//a more accurate method is needed at a later time
	
	//make sure can't go too fast in x direction
	if (velocity.x > maxSpeed) {
		velocity.x = maxSpeed;
	}
	if (velocity.x < -maxSpeed) {
		velocity.x = -maxSpeed;
	}
	
	velocity.y += incY;
	
	//make sure can't go too fast in y direction
	if (velocity.y > maxSpeed) {
		velocity.y = maxSpeed;
	}
	if (velocity.y < -maxSpeed) {
		velocity.y = -maxSpeed;
	}
	
}

void Ship::moveShip() {
	//move the ship
	position.x += velocity.x;
	position.y += velocity.y;
	
	//hw does wrap around for us
}

void Ship::reverseTurn() {
	angle = (2 * PI) - atan2(velocity.x, velocity.y);
}

void Ship::turnClockwise() {
	angle += turnSpeed;
}

void Ship::turnCounterClockwise() {
	angle -= turnSpeed;
}

Coordinate Ship::getPosition() {
	return position;
}

int Ship::getAngleDeg512() {
	return radToDeg512(angle);
}
