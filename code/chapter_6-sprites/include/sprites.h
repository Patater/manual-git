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

#define NUM_SPRITES 128
#define NUM_ROTATION_SPRITES 32

#define SPRITE_X_COORD_MASK 0x01FF
#define SPRITE_Y_COORD_MASK 0x00FF
#define SPRITE_ANGLE_MASK 0x01FF
#define ATTR2_PRIORTY_MASK ATTR2_PRIORITY(3);

/*
 * updateOAM
 *
 * Update the oAM
 *
 */
void updateOAM(SpriteEntry * spriteEntry);

/*
 * initOAM
 *
 * Initialize the OAM
 *
 */
void initOAM(SpriteEntry * spriteEntry, SpriteRotation * spriteRotation);

/*
 * moveSprite
 *
 * Move a sprite
 *
 */
void moveSprite(SpriteEntry * spriteEntry, u16 x, u16 y);

/*
 * rotateSprite
 *
 * Rotate a sprite.
 *
 */
void rotateSprite(SpriteRotation * spriteRotation, u16 angle);

/*
 * hideSprite
 *
 * Hide a sprite.
 *
 */
void hideSprite(SpriteEntry * spriteEntry, bool hide);

/*
 * setSpritePriority
 *
 * Set the priority of a sprite.
 *
 */
void setSpritePriority(SpriteEntry * spriteEntry, tObjPriority priority);

#endif
