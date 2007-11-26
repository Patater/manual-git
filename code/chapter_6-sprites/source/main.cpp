#include <nds.h>

#include "starField.h"
#include "planet.h"
#include "splash.h"

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
     */
    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

    /*  Set the video mode on the main screen. */
    videoSetMode(MODE_5_2D | // Set the graphics mode to Mode 5
                 DISPLAY_BG2_ACTIVE | // Enable BG2 for display
                 DISPLAY_BG3_ACTIVE); //Enable BG3 for display

    /*  Set the video mode on the sub screen. */
    videoSetModeSub(MODE_5_2D | // Set the graphics mode to Mode 5
                    DISPLAY_BG3_ACTIVE); // Enable BG3 for display
}

void initBackgrounds() {
    /*  Set up affine background 3 on main as a 16-bit color background */
    BG3_CR = BG_BMP16_256x256 |
             BG_BMP_BASE(0) | // The starting place in memory
             BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    BG3_XDX = 1 << 8;
    BG3_XDY = 0;
    BG3_YDX = 0;
    BG3_YDY = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the screen)
     */
    BG3_CX = 0;
    BG3_CY = 0;


    /*  Set up affine background 2 on main as a 16-bit color background */
    BG2_CR = BG_BMP16_128x128 |
             BG_BMP_BASE(8) | // The starting place in memory
             BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    BG2_XDX = 1 << 8;
    BG2_XDY = 0;
    BG2_YDX = 0;
    BG2_YDY = 1 << 8;

    /*  Place main screen background 2 an interesting place
     */
    BG2_CX = -(SCREEN_WIDTH / 2 - 32) << 8;
    BG2_CY = -32 << 8;

    /*  Set up affine background 3 on sub as a 16-bit color background */
    SUB_BG3_CR = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3
     *  to be the identity matrix.
     */
    SUB_BG3_XDX = 1 << 8;
    SUB_BG3_XDY = 0;
    SUB_BG3_YDX = 0;
    SUB_BG3_YDY = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    SUB_BG3_CX = 0;
    SUB_BG3_CY = 0;
}

void displayStarField() {
    dmaCopy(starFieldBitmap, // This variable is generated for us by grit
            (uint16 *)BG_BMP_RAM(0), // Our address for main background 3
            starFieldBitmapLen);
}

void displayPlanet() {
    dmaCopy(planetBitmap, // This variable is generated for us by grit
            (uint16 *)BG_BMP_RAM(8), // Our address for main background 2
            planetBitmapLen);
}

void displaySplash() {
    dmaCopy(splashBitmap, //This variable is generated for us by grit
            (uint16 *)BG_BMP_RAM_SUB(0), // Our address for sub background 3
            splashBitmapLen);
}

int main() {
    /*  Turn on the 2D graphics core. */
    powerON(POWER_ALL_2D);

    /*
     *  Set up interrupts.
     *
     *  We don't really get into what these do exactly at this point in the
     *  manual, but we still need to do them for now.
     */
    irqInit();
    irqSet(IRQ_VBLANK, 0);

    /*  Configure the VRAM and background control registers */
    lcdMainOnBottom(); // Place the main screen on the bottom physical screen
    initVideo(); 
    initBackgrounds(); 

    /*  Display the backgrounds */
    displayStarField(); 
    displayPlanet();
    displaySplash();

    return 0;
}
