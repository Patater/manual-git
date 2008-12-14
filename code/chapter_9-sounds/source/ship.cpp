/*
 *  Ship.cpp
 *  
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <math.h>
#include "ship.h"

int Ship::radToDeg(float rad) {
	return (int)(rad * (DEGREES_IN_CIRCLE/(2 *PI)));
}

Ship::Ship(SpriteInfo * _spriteInfo) {
    spriteInfo = _spriteInfo;
	
    /* Place the ship in an interesting part of the screen. */
	position.x = SCREEN_WIDTH / 2 - spriteInfo->width * 2 +
                 spriteInfo->width / 2;
	position.y = SCREEN_HEIGHT / 2 - spriteInfo->height;
	
    /* Stop the ship from moving */
	velocity.x = 0;
	velocity.y = 0;

    /* Point the ship at a cool angle. */
	angle = 5.67232007;
	
    /* Set up some sane kinematic static properties. */
	turnSpeed = .0368;
	thrust = .05;
	maxSpeed = 1.0;
	mass = 1.0;
}

Ship::~Ship() {
	//nothing (since we aren't taking ownership of the SpriteInfo struct pointed to by spriteInfo)
}

void Ship::accelerate() {
	
	float incX = thrust * sin(angle);
	float incY = -(thrust * cos(angle));
	
    //the following method of speed limitation is not accurate, traveling
    //diagonally is faster than straight, which is not the desired limitation
	//a more accurate method is needed at a later time
	
	velocity.x += incX;
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
	
    //hw does wrap around for us, so we don't have to have any of that sort of
    //logic in here
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

MathVector2D<float> Ship::getPosition() {
	return position;
}

int Ship::getAngleDeg() {
	return radToDeg(angle);
}
