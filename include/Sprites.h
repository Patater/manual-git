/*
 *  Sprites.h
 *  
 *	Adapted from Liran Nuna's sprite handling code. Special thanks to Liran Nuna.
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>

#ifndef SPRITES_H
#define SPRITES_H

//Update the OAM
void updateOAM(SpriteEntry * spriteEntry);

//Initialize the OAM
void initOAM(SpriteEntry * spriteEntry, SpriteRotation * spriteRotation);

//Move a Sprite
void moveSprite(SpriteEntry * spriteEntry, u16 x, u16 y);

//Rotate a Sprite
void rotateSprite(SpriteRotation * spriteRotation, u16 angle);

// Hide a Sprite
void hideSprite(SpriteEntry * spriteEntry, bool hide);

void setSpritePriority(SpriteEntry * spriteEntry, int priority);

#endif
