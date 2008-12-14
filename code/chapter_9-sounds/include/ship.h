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
#include "sprites.h"

#define PI (3.14159265358979323846264338327)

using namespace std;

template <class T>
struct MathVector2D {
    T x;
    T y;
};

class Ship {
protected:
    /*
     *  Game Engine Interfae
     *
     *  This variable provides an interface into the game engine. It connects this game object to a sprite via a SpriteInfo struct.
     *
     */
    SpriteInfo * spriteInfo;

    /* 
     *  Kinematic Dynamics
     *
     *  These variables track dynamic kinematic properties of the ship.
     *  Position and velocity is stored as two dimensional floating-point
     *  vectors (in the mathematical context, not the STL context). The angle
     *  of the ship is stored in radians in a float.
     */
    MathVector2D<float> position;
    MathVector2D<float> velocity;
    float angle; //in radians
 
    /*
     *  Kinematic Statics
     *
     *  These variables rarely change. When modified, they can change how the
     *  ship handles. The turning speed is stored in radians as a float. It
     *  should be a relatively small portion of 2 * PI. Thrust, maximum speed,
     *  and the mass of the ship are each stored as floats. 
     */
    float turnSpeed;
    float thrust;
    float maxSpeed;
    float mass;
 
    /*
     *  radToDeg
     *
     *  This function converts radians (our unit of choice for measuring
     *  angles) to the libnds degree system. We only use this function from
     *  within the Ship object, so we can make this a protected function.
     */
    int radToDeg(float rad);
 
    void init(const Ship & other);

public:
    /*  
     *  Ship
     *
     *  This is the default constructor. It requires a SpriteInfo struct in
     *  order to work correctly.
     *
     */
    Ship(SpriteInfo * _spriteInfo);

    /*
     *  ~Ship
     *
     *  This is the destructor. It cleans up the Ship object. It doesn't delete
     *  the pointer to SpriteInfo struct, since the Ship object does not assume
     *  ownership of the SpriteInfo struct.
     *
     */
    virtual ~Ship();
 
    /*
     *  accelerate
     *
     *  Accelerate the ship by adding the ship's thrust to it's velocity.
     *
     */
    void accelerate();

    /*
     *  moveShip
     *
     *  Move the ship by adding its velocity to its position.
     *
     */
    void moveShip();

    /*
     *  reverseTurn
     *
     *  Position the ship towards the opposite direction its velocity vector is
     *  pointing.
     *
     */
    void reverseTurn();

    /*
     *  turnClockwise
     *
     *  Turn the ship clockwise by the ship's turn speed.
     *
     */
    void turnClockwise();

    /*
     *  turnCounterClockwise
     *
     *  Turn the ship counter clockwise by the ship's turn speed.
     *
     */
    void turnCounterClockwise();

    /*
     *  getPosition
     *
     *  Returns the MathVector2D<float> representing the ship's position.
     *
     */
    MathVector2D<float> getPosition();

    /*
     *  getAngleRad
     *
     *  Returns the float representing the ship's angle in radians.
     *
     */
    float getAngleRad();

    /*
     *  getAngleDeg
     *
     *  Returns the int representing the ship's angle in the libnds degree
     *  system.
     *
     */
    int getAngleDeg();

};

#endif
