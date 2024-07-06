// ANCHOR: headers_1
#include <nds.h>

// ANCHOR_END: headers_1
// ANCHOR: bg_includes
#include "planet.h"
#include "splash.h"
#include "starField.h"

// ANCHOR_END: bg_includes

// For error checking only
#include "declarations.h"

// ANCHOR: dma_channel
/* Select a low priority DMA channel to perform our background copying. */
static const int DMA_CHANNEL = 3;

// ANCHOR_END: dma_channel

// ANCHOR: init_video
void initVideo() {
  /*
   *  Map VRAM to display a background on the main and sub screens.
   *
   *  The vramSetPrimaryBanks function takes four arguments, one for each of
   *  the major VRAM banks. We can use it as shorthand for assigning values to
   *  each of the VRAM bank's control registers.
   *
   *  We map banks A and B to main screen background memory. This gives us
   *  256KB, which is a healthy amount for 16-bit graphics.
   *
   *  We map bank C to sub screen background memory.
   *
   *  We map bank D to LCD. This setting is generally used for when we aren't
   *  using a particular bank.
   */
  vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000,
                      VRAM_C_SUB_BG_0x06200000, VRAM_D_LCD);

  /*  Set the video mode on the main screen. */
  videoSetMode(MODE_5_2D |           // Set the graphics mode to Mode 5
               DISPLAY_BG2_ACTIVE |  // Enable BG2 for display
               DISPLAY_BG3_ACTIVE);  // Enable BG3 for display

  /*  Set the video mode on the sub screen. */
  videoSetModeSub(MODE_5_2D |           // Set the graphics mode to Mode 5
                  DISPLAY_BG3_ACTIVE);  // Enable BG3 for display
}
// ANCHOR_END: init_video

// ANCHOR: init_backgrounds
void initBackgrounds() {
  /*  Set up affine background 3 on main as a 16-bit color background. */
  REG_BG3CNT = BG_BMP16_256x256 |
               BG_BMP_BASE(0) |  // The starting place in memory
               BG_PRIORITY(3);   // A low priority

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
               BG_BMP_BASE(8) |  // The starting place in memory
               BG_PRIORITY(2);   // A higher priority

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
                   BG_BMP_BASE(0) |  // The starting place in memory
                   BG_PRIORITY(3);   // A low priority

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
// ANCHOR_END: init_backgrounds

// ANCHOR: display_backgrounds
void displayStarField() {
  dmaCopyHalfWords(DMA_CHANNEL, starFieldBitmap, /* This variable is generated
                                                  * for us by grit. */
                   (uint16 *)BG_BMP_RAM(0),      /* Our address for main
                                                  * background 3 */
                   starFieldBitmapLen); /* This length (in bytes) is generated
                                         * from grit. */
}

void displayPlanet() {
  dmaCopyHalfWords(DMA_CHANNEL, planetBitmap, /* This variable is generated for
                                               * us by grit. */
                   (uint16 *)BG_BMP_RAM(8),   /* Our address for main
                                               * background 2 */
                   planetBitmapLen); /* This length (in bytes) is generated
                                      * from grit. */
}

void displaySplash() {
  dmaCopyHalfWords(DMA_CHANNEL, splashBitmap, /* This variable is generated for
                                               * us by grit. */
                   (uint16 *)BG_BMP_RAM_SUB(0), /* Our address for sub
                                                 * background 3 */
                   splashBitmapLen); /* This length (in bytes) is generated
                                      * from grit. */
}
// ANCHOR_END: display_backgrounds

#if MAIN_LOOP_1
// ANCHOR: main_loop_1
int main() {
  /* Turn on the 2D graphics core. */
  powerOn(POWER_ALL_2D);

  /* Loop forever so that the Nintendo DS doesn't reboot upon program
   * completion. */
  for (;;)
    ;

  return 0;
}
// ANCHOR_END: main_loop_1
#else
// ANCHOR: main_loop_2
int main() {
  /*  Turn on the 2D graphics core. */
  powerOn(POWER_ALL_2D);

  /*  Configure the VRAM and background control registers. */
  lcdMainOnBottom();  // Place the main screen on the bottom physical screen
  initVideo();
  initBackgrounds();

  /*  Display the backgrounds. */
  displayStarField();
  displayPlanet();
  displaySplash();

  /* Loop forever so that the Nintendo DS doesn't reboot upon program
   * completion. */
  for (;;)
    ;

  return 0;
}
// ANCHOR_END: main_loop_2
#endif
