/*
 *  Sprites.cpp
 *
 *  Adapted from Liran Nuna's sprite handling code. Special thanks to Liran
 *  Nuna.
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>
#include <nds/arm9/trig_lut.h>
#include "sprites.h"

void updateOAM(OAMTable * oam) {
    DC_FlushAll();
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     oam->oamBuffer,
                     OAM,
                     SPRITE_COUNT * sizeof(SpriteEntry));
}

void initOAM(OAMTable * oam) {
    /*
     * For all 128 sprites on the DS, disable and clear any attributes they
     * might have. This prevents any garbage from being displayed and gives us
     * a clean slate to work with.
     */
    for (int i = 0; i < SPRITE_COUNT; i++) {
        oam->oamBuffer[i].attribute[0] = ATTR0_DISABLED;
        oam->oamBuffer[i].attribute[1] = 0;
        oam->oamBuffer[i].attribute[2] = 0;
    }
    for (int i = 0; i < MATRIX_COUNT; i++) {
        /* If you look carefully, you'll see this is that affine trasformation
         * matrix again. We initialize it to the identity matrix, as we did
         * with backgrounds.
         */
        oam->matrixBuffer[i].hdx = 1 << 8;
        oam->matrixBuffer[i].hdy = 0;
        oam->matrixBuffer[i].vdx = 0;
        oam->matrixBuffer[i].vdy = 1 << 8;
    }
    updateOAM(oam);
}

void rotateSprite(SpriteRotation * spriteRotation, int angle) {
    s16 s = sinLerp(angle) >> 4;
    s16 c = cosLerp(angle) >> 4;

    spriteRotation->hdx = c;
    spriteRotation->hdy = s;
    spriteRotation->vdx = -s;
    spriteRotation->vdy = c;
}

void setSpriteVisibility(SpriteEntry * spriteEntry, bool hidden, bool affine,
                         bool doubleBound) {
    if (hidden) {
        /*
         * Make the sprite invisible.
         *
         * An affine sprite cannot be hidden. We have to turn it into a
         * non-affine sprite before we can hide it. To hide any sprite, we must
         * set bit 8 and clear bit 9. For non-affine sprites, this is a bit
         * redundant, but it is faster than a branch to just set it regardless
         * of whether or not it is already set.
         */
        spriteEntry->isRotateScale = false; // Bit 9 off
        spriteEntry->isHidden = true; // Bit 8 on
    } else {
        /* Make the sprite visible.*/
        if (affine) {
            /* Again, keep in mind that affine sprites cannot be hidden, so
             * enabling affine is enough to show the sprite again. We also need
             * to allow the user to get the double bound flag in the sprite
             * attribute. If we did not, then our sprite hiding function would
             * not be able to properly hide and restore double bound sprites.
             * We enable bit 9 here because we want an affine sprite.
             */
            spriteEntry->isRotateScale = true;

            /* The double bound flag only acts as the double bound flag when
             * the sprite is an affine sprite. At all other times, it acts as
             * the sprite invisibility flag. We only enable bit 8 here if we
             * want a double bound sprite. */
            spriteEntry->isSizeDouble = doubleBound;
        } else {
            /* Bit 9 (the affine flag) will already be off here, so we don't
             * need to clear it. However, bit 8 (the sprite invisibility flag)
             * will need to be cleared. */
            spriteEntry->isHidden = false;
        }
    }
}
