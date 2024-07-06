// ANCHOR: headers
#include <assert.h>
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
// ANCHOR_END: headers

#if MAIN_1
// ANCHOR: main_1
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
  iniOAMTable(oam);
  initSprites(oam, spriteInfo);

  /* Display the backgrounds. */
  displayStarField();
  displayPlanet();
  displaySplash();

  /* Make the ship object */
  static const int SHUTTLE_OAM_ID = 0;
  SpriteEntry *shipEntry = &oam->oamBuffer[SHUTTLE_OAM_ID];
  SpriteRotation *shipRotation = &oam->matrixBuffer[SHUTTLE_OAM_ID];
  Ship *ship = new Ship(&spriteInfo[SHUTTLE_OAM_ID]);

  /* Accelerate the ship for a little while to make it move. */
  for (int i = 0; i < 10; i++) {
    ship->accelerate();
  }

  /*
   *  Update the OAM.
   *
   *  We have to copy our copy of OAM data into the actual
   *  OAM during VBlank (writes to it are locked during
   *  other times).
   */
  swiWaitForVBlank();
  updateOAM(oam);

  /* Loop forever so that the Nintendo DS doesn't reboot upon program
   * completion. */
  for (;;)
    ;

  return 0;
}
// ANCHOR_END: main_1
#else
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
  iniOAMTable(oam);
  initSprites(oam, spriteInfo);

  /* Display the backgrounds. */
  displayStarField();
  displayPlanet();
  displaySplash();

  /* Make the ship object */
  static const int SHUTTLE_OAM_ID = 0;
  SpriteEntry *shipEntry = &oam->oamBuffer[SHUTTLE_OAM_ID];
  SpriteRotation *shipRotation = &oam->matrixBuffer[SHUTTLE_OAM_ID];
  Ship *ship = new Ship(&spriteInfo[SHUTTLE_OAM_ID]);

  /* Accelerate the ship for a little while to make it move. */
  for (int i = 0; i < 10; i++) {
    ship->accelerate();
  }

  // ANCHOR: main_loop
  for (;;) {
    /* Update the game state. */
    ship->moveShip();

    /* Update sprite attributes. */
    MathVector2D<float> position = ship->getPosition();
    shipEntry->x = (int)position.x;
    shipEntry->y = (int)position.y;
    rotateSprite(shipRotation, -ship->getAngleDeg());

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
// ANCHOR_END: main_loop
#endif
