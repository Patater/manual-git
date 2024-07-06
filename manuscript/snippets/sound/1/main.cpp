// ANCHOR: headers
#include <assert.h>
#include <maxmod9.h>
#include <nds.h>

#include "ship.h"
#include "sprites.h"

/* Backgrounds */
#include "planet.h"
#include "splash.h"
#include "starField.h"
/* Sprites */
#include "moon.h"
#include "orangeShuttle.h"
/* Sounds */
#include "soundbank.h"
#include "soundbank_bin.h"
// ANCHOR_END: headers

// ANCHOR: sound
void handleInput(Ship *ship, MathVector2D<int> *moonPos, SpriteInfo *moonInfo,
                 touchPosition *touch) {
  /* Handle up and down parts of D-Pad. */
  if (keysDown() & KEY_UP) {
    // Play our sound only when the button is initially pressed
    mmEffect(SFX_THRUST);
  }
  if (keysHeld() & KEY_UP) {
    // accelerate ship
    ship->accelerate();
  } else if (keysHeld() & KEY_DOWN) {
    // reverse ship direction
    ship->reverseTurn();
  }

  /* Handle left and right parts of D-Pad. */
  if (keysHeld() & KEY_LEFT) {
    // rotate counter clockwise
    ship->turnCounterClockwise();
  } else if (keysHeld() & KEY_RIGHT) {
    // rotate clockwise
    ship->turnClockwise();
  }

  /* Handle the touch screen. */
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

  /* Initialize maxmod using the memory based soundbank set up. */
  mmInitDefaultMem((mm_addr)soundbank_bin);

  /* Set up a few sprites. */
  SpriteInfo spriteInfo[SPRITE_COUNT];
  OAMTable *oam = new OAMTable();
  iniOAMTable(oam);
  initSprites(oam, spriteInfo);

  /* Display the backgrounds. */
  displayStarField();
  displayPlanet();
  displaySplash();

  /*************************************************************************/

  /* Keep track of the touch screen coordinates */
  touchPosition touch;

  /* Make the ship object */
  static const int SHUTTLE_AFFINE_ID = 0;
  SpriteEntry *shipEntry = &oam->oamBuffer[SHUTTLE_AFFINE_ID];
  SpriteRotation *shipRotation = &oam->matrixBuffer[SHUTTLE_AFFINE_ID];
  Ship *ship = new Ship(&spriteInfo[SHUTTLE_AFFINE_ID]);

  /* Make the moon */
  static const int MOON_AFFINE_ID = 1;
  SpriteEntry *moonEntry = &oam->oamBuffer[MOON_AFFINE_ID];
  SpriteInfo *moonInfo = &spriteInfo[MOON_AFFINE_ID];
  MathVector2D<int> *moonPos = new MathVector2D<int>();
  moonPos->x = moonEntry->x;
  moonPos->y = moonEntry->y;

  /*  Set up sound data.*/
  mmLoadEffect(SFX_THRUST);

  for (;;) {
    /* Update the game state. */
    updateInput(&touch);
    handleInput(ship, moonPos, moonInfo, &touch);
    ship->moveShip();

    /* Update ship sprite attributes. */
    MathVector2D<float> position = ship->getPosition();
    shipEntry->x = (int)position.x;
    shipEntry->y = (int)position.y;
    rotateSprite(shipRotation, -ship->getAngleDeg());
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
// ANCHOR_END: sound
