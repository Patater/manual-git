/*
 *  Sprites.h
 *  
 *	Adapted from Liran Nuna's sprite handling code. Special thanks to Liran
 *	Nuna.
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITE_ANGLE_MASK 0x01FF

/*
 *  updateOAM
 *
 *  Update the OAM.
 *
 */
void updateOAM(tOAM * oam);

/*
 *  initOAM
 *
 *  Initialize the OAM.
 *
 */
void initOAM(tOAM * oam);

/*
 *  moveSprite
 *
 *  Move a sprite.
 *
 */
void moveSprite(SpriteEntry * spriteEntry, u16 x, u16 y);

/*
 *  rotateSprite
 *
 *  Rotate a sprite.
 *
 */
void rotateSprite(SpriteRotation * spriteRotation, u16 angle);

/*
 *  setSpriteVisibility
 *
 *  Hide or show a sprite of a certain type: affine double bound, affine
 *  non-double bound, or ordinary.
 *
 */
void setSpriteVisibility(SpriteEntry * spriteEntry, bool hidden,
                         bool affine = false, bool doubleBound = false);

#endif
