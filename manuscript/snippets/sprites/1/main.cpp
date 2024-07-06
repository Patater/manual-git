// ANCHOR: headers
#include <nds.h>
// ANCHOR: assert
#include <assert.h>
// ANCHOR_END: assert
#include "sprites.h"

/* Backgrounds */
#include "planet.h"
#include "splash.h"
#include "starField.h"
/* Sprites */
#include <assert.h>

#include "moon.h"
#include "orangeShuttle.h"
// ANCHOR_END: headers

// ANCHOR: update_oam
void updateOAM(OAMTable *oam) {
  DC_FlushAll();
  dmaCopyHalfWords(SPRITE_DMA_CHANNEL, oam->oamBuffer, OAM,
                   SPRITE_COUNT * sizeof(SpriteEntry));
}
// ANCHOR_END: update_oam

// ANCHOR: init_oam
void iniOAMTable(OAMTable *oam) {
  /*
   * For all 128 sprites on the DS, disable and clear any attributes they
   * might have. This prevents any garbage from being displayed and gives
   * us a clean slate to work with.
   */
  for (int i = 0; i < SPRITE_COUNT; i++) {
    oam->oamBuffer[i].attribute[0] = ATTR0_DISABLED;
    oam->oamBuffer[i].attribute[1] = 0;
    oam->oamBuffer[i].attribute[2] = 0;
  }
  for (int i = 0; i < MATRIX_COUNT; i++) {
    /* If you look carefully, you'll see this is that affine trasformation
     * matrix again. We initialize it to the identity matrix, as we did
     * with backgrounds
     */
    oam->matrixBuffer[i].hdx = 1 << 8;
    oam->matrixBuffer[i].hdy = 0;
    oam->matrixBuffer[i].vdx = 0;
    oam->matrixBuffer[i].vdy = 1 << 8;
  }
  updateOAM(oam);
}
// ANCHOR_END: init_oam

// ANCHOR: rotate_sprite
void rotateSprite(SpriteRotation *spriteRotation, int angle) {
  s16 s = sinLerp(angle) >> 4;
  s16 c = cosLerp(angle) >> 4;

  spriteRotation->hdx = c;
  spriteRotation->hdy = s;
  spriteRotation->vdx = -s;
  spriteRotation->vdy = c;
}
// ANCHOR_END: rotate_sprite

// ANCHOR: sprite_visibility
void setSpriteVisibility(SpriteEntry *spriteEntry, bool hidden, bool affine,
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
    spriteEntry->isRotateScale = false;  // Bit 9 off
    spriteEntry->isHidden = true;        // Bit 8 on
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
       * the sprite invisibility flag. We only enable bit 8 here if we want
       * a double bound sprite. */
      spriteEntry->isSizeDouble = doubleBound;
    } else {
      /* Bit 9 (the affine flag) will already be off here, so we don't
       * need to clear it. However, bit 8 (the sprite invisibility flag)
       * will need to be cleared. */
      spriteEntry->isHidden = false;
    }
  }
}
// ANCHOR_END: sprite_visibility

void demo(void) {
  // ANCHOR: moving_sprites
  /* This is what we'd do if we wanted to move a sprite. */
  spriteEntry->x = 0;
  spriteEntry->y = 0;
  // ANCHOR_END: moving_sprites
  // ANCHOR: setting_priority
  spriteEntry->priority = OBJPRIORITY_3;
  // ANCHOR_END: setting_priority
  // ANCHOR: tile_computation
  static const int BOUNDARY_VALUE = 32; /* This is the default boundary value
                                         * (can be set in REG_DISPCNT) */
  static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);
  uint16 *tileVramAddress = &SPRITE_GFX[shuttle->gfxIndex * OFFSET_MULTIPLIER];
  // ANCHOR_END: tile_computation
}

// ANCHOR: init_video
void initVideo() {
  /*
   *  Map VRAM to display a background on the main and sub screens.
   *
   *  The vramSetPrimaryBanks function takes four arguments, one for each of
   *  the major VRAM banks. We can use it as shorthand for assigning values to
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
  vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000,
                      VRAM_C_SUB_BG_0x06200000, VRAM_D_LCD);

  vramSetBankE(VRAM_E_MAIN_SPRITE);

  /*  Set the video mode on the main screen. */
  videoSetMode(MODE_5_2D |           // Set the graphics mode to Mode 5
               DISPLAY_BG2_ACTIVE |  // Enable BG2 for display
               DISPLAY_BG3_ACTIVE |  // Enable BG3 for display
               DISPLAY_SPR_ACTIVE |  // Enable sprites for display
               DISPLAY_SPR_1D        // Enable 1D tiled sprites
  );

  /*  Set the video mode on the sub screen. */
  videoSetModeSub(MODE_5_2D |           // Set the graphics mode to Mode 5
                  DISPLAY_BG3_ACTIVE);  // Enable BG3 for display
}
// ANCHOR_END: init_video

// ANCHOR: init_sprites
void initSprites(OAMTable *oam, SpriteInfo *spriteInfo) {
  /*  Define some sprite configuration specific constants.
   *
   *  We will use these to compute the proper index into memory for certain
   *  tiles or palettes.
   *
   *  OFFSET_MULTIPLIER is calculated based on the following formula from
   *  GBATEK (http://nocash.emubase.de/gbatek.htm#dsvideoobjs):
   *    TileVramAddress = TileNumber * BoundaryValue
   *  Since SPRITE_GFX is a uint16*, the compiler will increment the address
   *  it points to by 2 for each change in 1 of the array index into
   *  SPRITE_GFX. (The compiler does pointer arithmetic.)
   */
  static const int BYTES_PER_16_COLOR_TILE = 32;
  static const int COLORS_PER_PALETTE = 16;
  static const int BOUNDARY_VALUE = 32; /* This is the default boundary value
                                         * (can be set in REG_DISPCNT) */
  static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);

  /* Keep track of the available tiles */
  int nextAvailableTileIdx = 0;

  /* Create the ship sprite. */
  static const int SHUTTLE_OAM_ID = 0;
  assert(SHUTTLE_OAM_ID < SPRITE_COUNT);
  SpriteInfo *shuttleInfo = &spriteInfo[SHUTTLE_OAM_ID];
  SpriteEntry *shuttle = &oam->oamBuffer[SHUTTLE_OAM_ID];

  /* Initialize shuttleInfo */
  shuttleInfo->oamId = SHUTTLE_OAM_ID;
  shuttleInfo->width = 64;
  shuttleInfo->height = 64;
  shuttleInfo->angle = 462;
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
   *  rotationIndex refers to the loation of affine transformation matrix. We
   *  set it to a location computed with a macro. OBJSIZE_64, in our case
   *  since we are making a square sprite, creates a 64x64 sprite.
   */
  shuttle->x =
      SCREEN_WIDTH / 2 - shuttleInfo->width * 2 + shuttleInfo->width / 2;
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
  rotateSprite(&oam->matrixBuffer[shuttleInfo->oamId], shuttleInfo->angle);

  /*************************************************************************/

  /* Create the moon sprite. */
  static const int MOON_OAM_ID = 1;
  assert(MOON_OAM_ID < SPRITE_COUNT);
  SpriteInfo *moonInfo = &spriteInfo[MOON_OAM_ID];
  SpriteEntry *moon = &oam->oamBuffer[MOON_OAM_ID];

  /* Initialize moonInfo */
  moonInfo->oamId = MOON_OAM_ID;
  moonInfo->width = 32;
  moonInfo->height = 32;
  moonInfo->angle = 462;
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
   * doesn't have an rotationIndex anymore. Instead, it has the ability to
   * flip the sprite vertically or horizontally.
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
  dmaCopyHalfWords(SPRITE_DMA_CHANNEL, orangeShuttlePal,
                   &SPRITE_PALETTE[shuttleInfo->oamId * COLORS_PER_PALETTE],
                   orangeShuttlePalLen);
  dmaCopyHalfWords(SPRITE_DMA_CHANNEL, moonPal,
                   &SPRITE_PALETTE[moonInfo->oamId * COLORS_PER_PALETTE],
                   moonPalLen);

  /* Copy the sprite graphics to sprite graphics memory */
  dmaCopyHalfWords(SPRITE_DMA_CHANNEL, orangeShuttleTiles,
                   &SPRITE_GFX[shuttle->gfxIndex * OFFSET_MULTIPLIER],
                   orangeShuttleTilesLen);
  dmaCopyHalfWords(SPRITE_DMA_CHANNEL, moonTiles,
                   &SPRITE_GFX[moon->gfxIndex * OFFSET_MULTIPLIER],
                   moonTilesLen);
}
// ANCHOR_END: init_sprites

// ANCHOR: main
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
// ANCHOR_END: main
