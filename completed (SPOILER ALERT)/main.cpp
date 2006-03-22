/*
 *  main.cpp
 *  
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>

//gfx
#include "OrangeShuttle_bin.h"
#include "OrangeShuttlePalette_bin.h"
#include "StarField_bin.h"
#include "Splash_bin.h"

#include "Sprites.h"
#include "Coordinate.h"
#include "Ship.h"

void updateInput() {
	scanKeys();
}

void handleInput(Ship * ship) {
	
	//up
	if ((keysDown() & KEY_UP) || (keysHeld() & KEY_UP)) {
		//accelerate ship
		ship->accelerate();
    }
	
	//down
	if ((keysDown() & KEY_DOWN) || (keysHeld() & KEY_DOWN)) {
		//reverse ship direction
		ship->reverseTurn();
    }
	
	//left
	if ((keysDown() & KEY_LEFT) || (keysHeld() & KEY_LEFT)) {
		//rotate counter clockwise
		ship->turnCounterClockwise();
    }

	//right
	if ((keysDown() & KEY_RIGHT) || (keysHeld() & KEY_RIGHT)) {
		//rotate clockwise
		ship->turnClockwise();
	}
	
	return;
}

void displaySplash() {
	dmaCopy(Splash_bin, (uint16 *)BG_BMP_RAM_SUB(3), Splash_bin_size);
}

void displayStarField() {
	dmaCopy(StarField_bin, (uint16 *)BG_BMP_RAM(3), StarField_bin_size);
}

void initBackgrounds() {
	//setup exrot bg 3 on main as a 16bit color background
	BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(3) | BG_PRIORITY(3);
	//attributes of the affine translation matrix
	BG3_XDX = 1 << 8; //scale x
	BG3_XDY = 0; //rotation x
	BG3_YDX = 0; //rotation y
	BG3_YDY = 1 << 8; //scale y
	BG3_CX = 0; //translation x
	BG3_CY = 0; //translation y
	
	//setup exrot bg 3 on sub
	SUB_BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(3) | BG_PRIORITY(3);
	//attributes of the affine translation matrix
	SUB_BG3_XDX = 1 << 8; //scale x
	SUB_BG3_XDY = 0; //rotation x
	SUB_BG3_YDX = 0; //scale y
	SUB_BG3_YDY = 1 << 8; //scale y
	SUB_BG3_CX = 0; //translation x
	SUB_BG3_CY = 0; //translation y
}

void initVideo() {
	//enable vram and map it to the right places
    vramSetMainBanks(   VRAM_A_MAIN_SPRITE,        //A and B maped consecutively as sprite memory
                        VRAM_B_MAIN_SPRITE,        //this gives us 256KB which is the max
                        VRAM_C_SUB_BG_0x6200000,   //map C to sub background memory
                        VRAM_D_MAIN_BG_0x6000000   //mac C to main background memory
                        );
	
	
	//set the video mode
    videoSetMode(  MODE_5_2D | 
                   DISPLAY_SPR_ACTIVE |    //turn on sprites
                   DISPLAY_BG3_ACTIVE |    //turn on background 3
                   DISPLAY_SPR_1D          //this is used when in tile mode
				   );
	
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
}

void initSprites(Ship * ship, SpriteEntry * spriteEntry, SpriteRotation * spriteRotation) {
	//init OAM
	initOAM(spriteEntry, spriteRotation);
	
	//create the ship sprite
	int orangeShipGfxID = 64;
	
	Coordinate position = ship->getPosition();
	
	spriteEntry[0].attribute[0] = ATTR0_COLOR_256 | ATTR0_ROTSCALE_DOUBLE | (int)position.y;
	spriteEntry[0].attribute[1] = ATTR1_ROTDATA(0) | ATTR1_SIZE_64 | (int)position.x; // size 64x64
	spriteEntry[0].attribute[2] = orangeShipGfxID;
	
	//set initial rotation attributes
	rotateSprite(&spriteRotation[0], ship->getAngleDeg512());
	
	//copy in the sprite palettes
	dmaCopy(OrangeShuttlePalette_bin, (uint16 *)SPRITE_PALETTE, OrangeShuttlePalette_bin_size);
	
	//copy the sprite grahics in obj graphics mem
	/*
    for(unsigned int i = 0; i < OrangeShuttle_bin_size << 16; i++) {
		SPRITE_GFX[orangeShipGfxID * 16 + i] = ((uint16*)OrangeShuttle_bin)[i];
		SPRITE_GFX_SUB[orangeShipGfxID * 16 + i] = ((uint16*)OrangeShuttle_bin)[i];
	}
	*/
	
	DMA_SRC(3) = ((uint32 *)OrangeShuttle_bin);
	DMA_DST(3) = ((uint32 *)SPRITE_GFX) + (orangeShipGfxID << 4);
	DMA_CR(3) = DMA_ENABLE | DMA_32BIT | 4096;
    
}

int main() {
	
	//////////////////////////// INIT ////////////////////////////////////////
	//turn on the 2D core
    powerON(POWER_ALL_2D);
	
    //set up a VBlank handler
	irqInit();
	irqSet(IRQ_VBLANK, 0);
    
    initVideo();
	initBackgrounds();
	
	//display backgrounds
   	displayStarField();
	displaySplash();
	
	//make the ship
	Ship * ship = new Ship(64, 64);
	
	//////////////////////////// Sprite setup ////////////////////////////////
    //sprite attribute memory
	SpriteEntry * spritesMain = new SpriteEntry[128];
	//rotation attributes overlap so assign then to the same location
	SpriteRotation * spriteRotationsMain = (SpriteRotation *)spritesMain;
	
	//initSprites(ship, weapon, spritesMain, spriteRotationsMain);
	initSprites(ship, spritesMain, spriteRotationsMain);
	
	for (;;) {
		updateInput();
		
		handleInput(ship);
		
		//update sprite attributes (put inside a function for each sprite)
		ship->moveShip();
		Coordinate position = ship->getPosition();
		moveSprite(&spritesMain[0], (int)position.x, (int)position.y);
		rotateSprite(&spriteRotationsMain[0], ship->getAngleDeg512());
		
		swiWaitForVBlank();
		
		updateOAM(spritesMain);
	}
}
