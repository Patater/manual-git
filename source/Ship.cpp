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
	return 0;
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
	
}

void Ship::moveShip() {

}

void Ship::reverseTurn() {
	
}

void Ship::turnClockwise() {
	
}

void Ship::turnCounterClockwise() {
	
}

Coordinate Ship::getPosition() {
	return position;
}

int Ship::getAngleDeg512() {
	return radToDeg512(angle);
}
