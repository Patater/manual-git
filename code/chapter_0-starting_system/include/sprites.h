/*
 *  Sprites.h
 *
 *  Adapted from Liran Nuna's sprite handling code. Special thanks to Liran
 *  Nuna.
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>

#ifndef SPRITES_H
#define SPRITES_H

static const int SPRITE_DMA_CHANNEL = 3;

typedef struct {
    int oamId;
    int width;
    int height;
    int angle;
    SpriteEntry * entry;
} SpriteInfo;

/*
 *  updateOAM
 *
 *  Update the OAM.
 *
 */
void updateOAM(OAMTable * oam);

/*
 *  initOAM
 *
 *  Initialize the OAM.
 *
 */
void initOAM(OAMTable * oam);

/*
 *  rotateSprite
 *
 *  Rotate a sprite counter-clockwise by the specified angle (in degrees).
 *
 */
void rotateSprite(SpriteRotation * spriteRotation, int angle);

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
