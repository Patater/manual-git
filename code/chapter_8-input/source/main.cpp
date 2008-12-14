/*
 *  main.cpp
 *  
 *  Created by Jaeden Amero on 11/12/07.
 *  Copyright 2007. All rights reserved.
 *
 */

#include <nds.h>
#include <assert.h>
#include "sprites.h"
#include "ship.h"

/* Backgrounds */
#include "starField.h"
#include "planet.h"
#include "splash.h"
/* Sprites */
#include "orangeShuttle.h"
#include "moon.h"

/* Select a low priority DMA channel to perform our background copying. */
static const int DMA_CHANNEL = 3;

void initVideo() {
    /*
     *  Map VRAM to display a background on the main and sub screens.
     * 
     *  The vramSetMainBanks function takes four arguments, one for each of the
     *  major VRAM banks. We can use it as shorthand for assigning values to
     *  each of the VRAM bank's control registers.
     *
     *  We map banks A and B to main screen  background memory. This gives us
     *  256KB, which is a healthy amount for 16-bit graphics.
     *
     *  We map bank C to sub screen background memory.
     *
     *  We map bank D to LCD. This setting is generally used for when we aren't
     *  using a particular bank.
     *
     *  We map bank E to main screen sprite memory (aka object memory).
     */
    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);

    /*  Set the video mode on the main screen. */
    videoSetMode(MODE_5_2D | // Set the graphics mode to Mode 5
                 DISPLAY_BG2_ACTIVE | // Enable BG2 for display
                 DISPLAY_BG3_ACTIVE | // Enable BG3 for display
                 DISPLAY_SPR_ACTIVE | // Enable sprites for display
                 DISPLAY_SPR_1D       // Enable 1D tiled sprites
                 );

    /*  Set the video mode on the sub screen. */
    videoSetModeSub(MODE_5_2D | // Set the graphics mode to Mode 5
                    DISPLAY_BG3_ACTIVE); // Enable BG3 for display
}

void initBackgrounds() {
    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 2 on main as a 16-bit color background. */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(8) | // The starting place in memory
                 BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /*  Place main screen background 2 in an interesting place. */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    /*  Set up affine background 3 on the sub screen as a 16-bit color
     *  background.
     */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                     BG_BMP_BASE(0) | // The starting place in memory
                     BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}

void initSprites(OAMTable * oam, SpriteInfo *spriteInfo) {
    /*  Define some sprite configuration specific constants.
     * 
     *  We will use these to compute the proper index into memory for certain
     *  tiles or palettes.
     *
     *  OFFSET_MULTIPLIER is calculated based on the following formula from
     *  GBATEK (http://nocash.emubase.de/gbatek.htm#dsvideoobjs):
     *      TileVramAddress = TileNumber * BoundaryValue
     *  Since SPRITE_GFX is a uint16*, the compiler will increment the address
     *  it points to by 2 for each change in 1 of the array index into
     *  SPRITE_GFX. (The compiler does pointer arithmetic.)
     */
    static const int BYTES_PER_16_COLOR_TILE = 32;
    static const int COLORS_PER_PALETTE = 16;
    static const int BOUNDARY_VALUE = 32; /* This is the default boundary value
                                           * (can be set in REG_DISPCNT) */
    static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE /
                                         sizeof(SPRITE_GFX[0]);

    /* Keep track of the available tiles */
    int nextAvailableTileIdx = 0;

    /* Create the ship sprite. */
    static const int SHUTTLE_OAM_ID = 0;
    assert(SHUTTLE_OAM_ID < SPRITE_COUNT);
    SpriteInfo * shuttleInfo = &spriteInfo[SHUTTLE_OAM_ID];
    SpriteEntry * shuttle = &oam->oamBuffer[SHUTTLE_OAM_ID];

    /* Initialize shuttleInfo */
    shuttleInfo->oamId = SHUTTLE_OAM_ID;
    shuttleInfo->width = 64;
    shuttleInfo->height = 64;
    shuttleInfo->angle = 29568;
    shuttleInfo->entry = shuttle;

    /*
     *  Configure attribute 0. 
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We specify that we want an
     *  affine sprite (via isRotateScale) here because we would like to rotate
     *  the ship.
     */
    shuttle->y = SCREEN_HEIGHT / 2 - shuttleInfo->height;
    shuttle->isRotateScale = true;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!shuttle->isRotateScale || (shuttleInfo->oamId < MATRIX_COUNT));
    shuttle->isSizeDouble = false;
    shuttle->blendMode = OBJMODE_NORMAL;
    shuttle->isMosaic = false;
    shuttle->colorMode = OBJCOLOR_16;
    shuttle->shape = OBJSHAPE_SQUARE;

    /*
     *  Configure attribute 1.
     *
     *  rsMatrixId refers to the loation of affine transformation matrix. We
     *  set it to a location computed with a macro. OBJSIZE_64, in our case
     *  since we are making a square sprite, creates a 64x64 sprite.
     */
    shuttle->x = SCREEN_WIDTH / 2 - shuttleInfo->width * 2 +
                    shuttleInfo->width / 2;
    shuttle->rotationIndex = shuttleInfo->oamId;
    shuttle->size = OBJSIZE_64;

    /* 
     *  Configure attribute 2.
     * 
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    shuttle->gfxIndex = nextAvailableTileIdx;
    nextAvailableTileIdx += orangeShuttleTilesLen / BYTES_PER_16_COLOR_TILE;
    shuttle->priority = OBJPRIORITY_0;
    shuttle->palette = shuttleInfo->oamId;

    /* Rotate the sprite */
    rotateSprite(&oam->matrixBuffer[shuttleInfo->oamId],
                 shuttleInfo->angle);

    /*************************************************************************/

    /* Create the moon sprite. */
    static const int MOON_OAM_ID = 1;
    assert(MOON_OAM_ID < SPRITE_COUNT);
    SpriteInfo * moonInfo = &spriteInfo[MOON_OAM_ID];
    SpriteEntry * moon = &oam->oamBuffer[MOON_OAM_ID];

    /* Initialize moonInfo */
    moonInfo->oamId = MOON_OAM_ID;
    moonInfo->width = 32;
    moonInfo->height = 32;
    moonInfo->angle = 29568;
    moonInfo->entry = moon;

    /*
     *  Configure attribute 0.
     *
     *  OBJCOLOR_16 will make a 16-color sprite. We won't specify that we want
     *  an affine sprite here because we don't want one this time.
     */
    moon->y = SCREEN_WIDTH / 2 + moonInfo->height / 2;
    moon->isRotateScale = false;
    /* This assert is a check to see a matrix is available to store the affine
     * transformation matrix for this sprite. Of course, you don't have to have
     * the matrix id match the affine id, but if you do make them match, this
     * assert can be helpful. */
    assert(!moon->isRotateScale || (moonInfo->oamId < MATRIX_COUNT));
    moon->isHidden = false;
    moon->blendMode = OBJMODE_NORMAL;
    moon->isMosaic = false;
    moon->colorMode = OBJCOLOR_16;
    moon->shape = OBJSHAPE_SQUARE;

    /*
     * Configure attribute 1.
     *
     * OBJSIZE_32 will create a sprite of size 32x32, since we are making a
     * square sprite. Since we are using a non-affine sprite, attribute 1
     * doesn't have an rotationIndex anymore. Instead, it has the ability to flip
     * the sprite vertically or horizontally.
     */
    moon->x = SCREEN_WIDTH / 2 + moonInfo->width + moonInfo->width / 2;
    moon->hFlip = false;
    moon->vFlip = false;
    moon->size = OBJSIZE_32;

    /* 
     *  Configure attribute 2.
     * 
     *  Configure which tiles the sprite will use, which priority layer it will
     *  be placed onto, which palette the sprite should use, and whether or not
     *  to show the sprite.
     */
    moon->gfxIndex = nextAvailableTileIdx;
    nextAvailableTileIdx += moonTilesLen / BYTES_PER_16_COLOR_TILE;
    moon->priority = OBJPRIORITY_2;
    moon->palette = moonInfo->oamId;

    /*************************************************************************/

    /* Copy over the sprite palettes */
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     orangeShuttlePal,
                     &SPRITE_PALETTE[shuttleInfo->oamId *
                                     COLORS_PER_PALETTE],
                     orangeShuttlePalLen);
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     moonPal,
                     &SPRITE_PALETTE[moonInfo->oamId * COLORS_PER_PALETTE],
                     moonPalLen);

    /* Copy the sprite graphics to sprite graphics memory */
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     orangeShuttleTiles,
                     &SPRITE_GFX[shuttle->gfxIndex * OFFSET_MULTIPLIER],
                     orangeShuttleTilesLen);
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     moonTiles,
                     &SPRITE_GFX[moon->gfxIndex * OFFSET_MULTIPLIER],
                     moonTilesLen);
}

void displayStarField() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     starFieldBitmap, /* This variable is generated for us by
                                       * grit. */
                     (uint16 *)BG_BMP_RAM(0), /* Our address for main
                                               * background 3 */
                     starFieldBitmapLen);
}

void displayPlanet() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     planetBitmap, /* This variable is generated for us by
                                    * grit. */
                     (uint16 *)BG_BMP_RAM(8), /* Our address for main
                                               * background 2 */
                     planetBitmapLen);
}

void displaySplash() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     splashBitmap, /* This variable is generated for us by
                                    * grit. */
                     (uint16 *)BG_BMP_RAM_SUB(0), /* Our address for sub
                                                     background 3 */
                     splashBitmapLen);
}

void updateInput(touchPosition * touch) {
	// Update the key registers with current values.
	scanKeys();

    // Update the touch screen values.
    touchRead(touch);
}

void handleInput(Ship * ship, MathVector2D<int> * moonPos,
                 SpriteInfo * moonInfo, touchPosition * touch) {

    /* Handle up and down parts of D-Pad. */
	if (keysHeld() & KEY_UP) {
		//accelerate ship
		ship->accelerate();
	} else if (keysHeld() & KEY_DOWN) {
		//reverse ship direction
		ship->reverseTurn();
    }
	
    /* Handle left and right parts of D-Pad. */
	if (keysHeld() & KEY_LEFT) {
		//rotate counter clockwise
		ship->turnCounterClockwise();
    } else if (keysHeld() & KEY_RIGHT) {
		//rotate clockwise
		ship->turnClockwise();
	}

    /*
     *  Handle the touch screen.
     *  
     *  This is basically some fancy pants junk to enable grabbing and moving
     *  of the moon. It isn't essential to know how this code works to
     *  understand how to reach values from the touch screen, but it was cool
     *  enough that I wanted to put it in the case study.
    */
    static MathVector2D<int> moonGrip;
    if (keysDown() & KEY_TOUCH) {
        /* Record the grip */
        moonGrip.x = touch->px;
        moonGrip.y = touch->py;
    } else if (keysHeld() & KEY_TOUCH) {
        int newX = moonPos->x + touch->px - moonGrip.x;
        int newY = moonPos->y + touch->py - moonGrip.y;

        /* Prevent dragging off the screen */
        if (newX < 0) {
            moonPos->x = 0;
        } else if (newX > (SCREEN_WIDTH - moonInfo->width)) {
            moonPos->x = SCREEN_WIDTH - moonInfo->width;
        } else {
            moonPos->x = newX;
        }
        if (newY < 0) {
            moonPos->y = 0;
        } else if (newY > (SCREEN_HEIGHT - moonInfo->height)) {
            moonPos->y = SCREEN_HEIGHT - moonInfo->height;
        } else {
            moonPos->y = newY;
        }

        /* Record the grip again. */
        moonGrip.x = touch->px;
        moonGrip.y = touch->py;
    }
}

int main() {
    /*  Turn on the 2D graphics core. */
    powerOn(POWER_ALL_2D);

    /*
     *  Configure the VRAM and background control registers.
     *
     *  Place the main screen on the bottom physical screen. Then arrange the
     *  VRAM banks. Next, confiure the background control registers.
     */
    lcdMainOnBottom();
    initVideo();
    initBackgrounds();

    /* Set up a few sprites. */
    SpriteInfo spriteInfo[SPRITE_COUNT];
    OAMTable *oam = new OAMTable();
    initOAM(oam);
    initSprites(oam, spriteInfo);

    /* Display the backgrounds. */
    displayStarField();
    displayPlanet();
    displaySplash();

    /*************************************************************************/

    /* Keep track of the touch screen coordinates */
    touchPosition touch;

    /* Make the ship object */
    static const int SHUTTLE_OAM_ID = 0;
    SpriteEntry * shipEntry = &oam->oamBuffer[SHUTTLE_OAM_ID];
    SpriteRotation * shipRotation = &oam->matrixBuffer[SHUTTLE_OAM_ID];
    Ship * ship = new Ship(&spriteInfo[SHUTTLE_OAM_ID]);

    /* Make the moon */
    static const int MOON_OAM_ID = 1;
    SpriteEntry * moonEntry = &oam->oamBuffer[MOON_OAM_ID];
    SpriteInfo * moonInfo = &spriteInfo[MOON_OAM_ID];
    MathVector2D<int> * moonPos = new MathVector2D<int>();
    moonPos->x = moonEntry->x;
    moonPos->y = moonEntry->y;

    for (;;) {
        /* Update the game state. */
		updateInput(&touch);
		handleInput(ship, moonPos, moonInfo, &touch);
        ship->moveShip();

        /* Update ship sprite attributes. */
        MathVector2D<float> position = ship->getPosition();
        shipEntry->x = (int)position.x;
        shipEntry->y = (int)position.y;
        rotateSprite(shipRotation, ship->getAngleDeg());
        /* Update moon sprite attributes. */
        moonEntry->x = (int)moonPos->x;
        moonEntry->y = (int)moonPos->y;

        /*
         *  Update the OAM.
         *
         *  We have to copy our copy of OAM data into the actual OAM during
         *  VBlank (writes to it are locked during other times).
         */
        swiWaitForVBlank();
        updateOAM(oam);
    }

    return 0;
}
