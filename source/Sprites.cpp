/*
 *  Sprites.cpp
 *  
 *	Adapted from Liran Nuna's sprite handling code. Special thanks to Liran Nuna.
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>
#include <nds/arm9/trig_lut.h>

//Update the OAM
void updateOAM(SpriteEntry * spriteEntry) {
	DC_FlushAll();
    dmaCopy(spriteEntry, OAM, 128 * sizeof(SpriteEntry));
    //dmaCopy(spritesSub,  OAM_SUB, 	128 * sizeof(SpriteEntry));
}

//Initialize the OAM
void initOAM(SpriteEntry * spriteEntry, SpriteRotation * spriteRotation) {
	//For all 128 sprites on the DS, disable and clear any attributes they 
	//might have. This prevents any garbage from being displayed and gives 
	//us a clean slate to work with.
	for(int i = 0; i < 128; i++) {
		spriteEntry[i].attribute[0] = ATTR0_DISABLED;
		spriteEntry[i].attribute[1] = 0;
		spriteEntry[i].attribute[2] = 0;
	}
	for(int i = 0; i < 32; i++) {
		spriteRotation[i].hdx = 256;
		spriteRotation[i].hdy = 0;
		spriteRotation[i].vdx = 0;
		spriteRotation[i].vdy = 256;
	}
	updateOAM(spriteEntry);
}

//Move a Sprite
void moveSprite(SpriteEntry * spriteEntry, u16 x, u16 y) {
	spriteEntry->attribute[1] &= 0xFE00;
	spriteEntry->attribute[1] |= (x & 0x01FF);
	
	spriteEntry->attribute[0] &= 0xFF00;
	spriteEntry->attribute[0] |= (y & 0x00FF);
} 

//Rotate a Sprite
void rotateSprite(SpriteRotation * spriteRotation, u16 angle) {
	s16 s = -SIN[angle & 0x1FF] >> 4;
	s16 c =  COS[angle & 0x1FF] >> 4;
	
	spriteRotation->hdx =  c;
	spriteRotation->hdy = -s;
	spriteRotation->vdx =  s;
	spriteRotation->vdy =  c;
	
	//maybe wrong? have somebody who knows what they are talking about explain tonc's explanation to you
	//for me, and according to tonc, this matrix seems wrong, but at the same time, it works
	//http://user.chem.tue.nl/jakvijn/tonc/affine.htm
}

// Hide a Sprite
void hideSprite(SpriteEntry * spriteEntry, bool hide) {
	if (hide) {
		spriteEntry->attribute[0] |= ATTR0_DISABLED;
	} else {
		spriteEntry->attribute[0] &= ~ATTR0_DISABLED;
	}
}

void setSpritePriority(SpriteEntry * spriteEntry, int priority) {
	spriteEntry->attribute[2] &= ~ATTR2_PRIORITY(3);
	spriteEntry->attribute[2] |= ATTR2_PRIORITY(priority);
}

