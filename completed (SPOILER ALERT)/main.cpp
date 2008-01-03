/*
 *  main.cpp
 *  
 *
 *  Created by Jaeden Amero on 3/12/06.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <nds.h>
//#include <nds/arm9/sound.h>

//gfx
#include "OrangeShuttle_bin.h"
#include "OrangeShuttlePalette_bin.h"
#include "StarField_bin.h"
#include "Splash_bin.h"
//snd
#include "thrust_1_22050_8_s_raw.h"

#include "Sprites.h"
#include "Coordinate.h"
#include "Ship.h"

void updateInput() {
	//updates the key registers with current values
	scanKeys();
}

void handleInput(Ship * ship, TransferSoundData * soundData) {
	
	//up
	if (keysDown() & KEY_UP) {
		//play our sound only when the button is initially pressed
		playSound(soundData);
    }
	if (keysHeld() & KEY_UP) {
		//accelerate ship
		ship->accelerate();
	}
	
	//down
	if (keysHeld() & KEY_DOWN) {
		//reverse ship direction
		ship->reverseTurn();
    }
	
	//left
	if (keysHeld() & KEY_LEFT) {
		//rotate counter clockwise
		ship->turnCounterClockwise();
    }

	//right
	if (keysHeld() & KEY_RIGHT) {
		//rotate clockwise
		ship->turnClockwise();
	}
	
	return;
}

void displaySplash() {
	dmaCopy(Splash_bin, (uint16 *)BG_BMP_RAM_SUB(0), Splash_bin_size);
}

void displayStarField() {
	dmaCopy(StarField_bin, (uint16 *)BG_BMP_RAM(0), StarField_bin_size);
}

void initBackgrounds() {
	//setup exrot bg 3 on main as a 16bit color background
	BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(0) | BG_PRIORITY(3);
	//attributes of the affine translation matrix
	BG3_XDX = 1 << 8; //scale x
	BG3_XDY = 0; //rotation x
	BG3_YDX = 0; //rotation y
	BG3_YDY = 1 << 8; //scale y
	BG3_CX = 0; //translation x
	BG3_CY = 0; //translation y
	
	//setup exrot bg 3 on sub
	SUB_BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(0) | BG_PRIORITY(3);
	//attributes of the affine translation matrix
	SUB_BG3_XDX = 1 << 8; //scale x
	SUB_BG3_XDY = 0; //rotation x
	SUB_BG3_YDX = 0; //rotation y
	SUB_BG3_YDY = 1 << 8; //scale y
	SUB_BG3_CX = 0; //translation x
	SUB_BG3_CY = 0; //translation y
}

void initVideo() {
	//enable vram and map it to the right places
    vramSetMainBanks(   VRAM_A_MAIN_BG_0x06000000,		//map A and B to main background memory
                        VRAM_B_MAIN_BG_0x06020000,		//this gives us 256KB which is a healthy amount for 16-bit gfx
                        VRAM_C_SUB_BG_0x06200000,		//map C to sub background memory
                        VRAM_D_LCD						//map D to LCD free space
						//allows adjacent banks to overflow into D if a bug like that was ever to occur
                        );
	
	//map a bank for use with sprites
	vramSetBankE(VRAM_E_MAIN_SPRITE); //mapping E to main sprites gives us 64k for sprites
	//(64k is the max space that 1024 tiles take up in 256 color mode)
	
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
	spriteEntry[0].attribute[1] = ATTR1_ROTDATA(0) | ATTR1_SIZE_64 | (int)position.x; //size 64x64
	spriteEntry[0].attribute[2] = orangeShipGfxID; /*64*/
	
	//set initial rotation attributes
	rotateSprite(&spriteRotation[0], ship->getAngleDeg512());
	
	//copy in the sprite palettes
	dmaCopy(OrangeShuttlePalette_bin, SPRITE_PALETTE, OrangeShuttlePalette_bin_size);
	
	//copy the sprite graphics in obj graphics mem
	dmaCopy(OrangeShuttle_bin, &SPRITE_GFX[orangeShipGfxID * 16 /*512*/], OrangeShuttle_bin_size);
	
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
	
	//////////////////////////// Sound Data setup ////////////////////////////
	TransferSoundData * thrust_sound = new TransferSoundData();
	thrust_sound->data = thrust_1_22050_8_s_raw;
	thrust_sound->len = thrust_1_22050_8_s_raw_size;
	thrust_sound->rate = 22050; //22050Hz Sample Rate
	thrust_sound->vol = 127; //Volume can be from 0 to 127
	thrust_sound->pan = 64; //Sound comes equally from both speakers (0-127, left to right)
	thrust_sound->format = 1; //1 - 8bit, 0 - 16bit
	
	for (;;) {
		updateInput();
		
		handleInput(ship, thrust_sound);
		
		//update sprite attributes (put inside a function for each sprite)
		ship->moveShip();
		Coordinate position = ship->getPosition();
		moveSprite(&spritesMain[0], (int)position.x, (int)position.y);
		rotateSprite(&spriteRotationsMain[0], ship->getAngleDeg512());
		
		swiWaitForVBlank();
		
		updateOAM(spritesMain);
	}
}
