# Chapter 8
## Nintendo DS Input Systems

### Overview

The Nintendo DS has many different user input systems, including buttons, touch
screen, and a microphone. Most video game systems only have buttons and an
analog stick or two. While the Nintendo DS does not have an analog stick, it
does have an amazing touch screen which has millions of different creative
uses. We will only cover the touch screen and buttons, though. If you wish to
learn more about the microphone, I'd recommend reading, although a bit
outdated, the only microphone resource I know of
<http://www.double.co.nz/nintendo_ds/nds_develop9.html>.

### Key Input

libnds provides us with a very nice abstraction for key input. Instead of
having to AND registers with cryptic masks to discover which keys we are
pressing, we simply call `scanKeys()`, then check one of three input functions,
`keysDown()`, `keysHeld()`, or `keysUp()`. In order to see which keys have been
recently pressed, use `keysDown()`. To see which keys are currently held, use
`keysHeld()`. To see which keys have just been released, use `keysUp()`. libnds
provides us with defines for some key masks as well. How they are set up is
explained in [Table 8.1, “libnds Key Defines”](#libnds_key_defines).

<a name="libnds_key_defines"></a>

#### Table 8.1. libnds Key Defines

| Key Define | Mask Bit | Associated Input |
| ---------- | -------- | ---------------- |
| `KEY_A` | `1 << 0` | A Button |
| `KEY_B` | `1 << 1` | B Button |
| `KEY_SELECT` | `1 << 2` | Select Button |
| `KEY_START` | `1 << 3` | Start Button |
| `KEY_RIGHT` | `1 << 4` | Right D-pad |
| `KEY_LEFT` | `1 << 5` | Left D-pad |
| `KEY_UP` | `1 << 6` | p D-pad |
| `KEY_DOWN` | `1 << 7` | Down D-pad |
| `KEY_R` | `1 << 8` | R Button |
| `KEY_L` | `1 << 9` | L Button |
| `KEY_X` | `1 << 10` | X Button |
| `KEY_Y` | `1 << 11` | Y Button |
| `KEY_TOUCH` | `1 << 12` | Pen Touching Screen (no coordinates) |
| `KEY_LID` | `1 << 13` | Lid shutting (useful for sleeping) |

### Touch!

Utada Hikaru (<https://www.youtube.com/watch?v=bYoVeIGl0ao>) wants to touch it.
You have to code it. Let's go!

The touch screen a big part of what makes the Nintendo DS awesome. The libnds
API for using it is cake easy too. We'll be ready for Utada Hikaru in no time.
Whenever you want to read the current touch location, simply call the function
`touchRead()`. This function assigns values to a struct that contains the x and
y coordinate of the touch. You use it like so.

```C
/* To read from the touch screen, we can do this stuff. */
touchPosition touch;
touchRead(&touch);
touch->px; // This contains the x pixel location.
touch->py; // This contains the y pixel location.
```

#### What is the IPC?

<!-- Note -->
You may see code that uses something called the IPC to interact with the touch
screen. Use of the IPC struct is deprecated. This means that you shouldn't use
it and should not depend on it being there in the future. The method we use to
read values from the touch screen does not use the IPC struct and is safe for
future use.

### Writing an Input Updating Function

Now that we know a bit about how input is laid out on the Nintendo DS, let's
write a function in our `main.cpp` to collect the input for us. We'll call it
`updateInput()`. We'll use this function as the first thing we call in our
complete game loop.

```C++
void updateInput(touchPosition * touch) {
  // Update the key registers with current values.
  scanKeys();

  // Update the touch screen values.
  touchRead(touch);
}
```

### Writing an Input Handling Function

This function will be part of our game state updating game loop component. It
will react to outside input and modify the game state accordingly. As such, it
will have to know about and be able to modify the game state. We'll pass the
game state to it as function parameters. Let's call the function
`handleInput()`.

First, we want the ship to accelerate when we press up. To do this, we detect
when the Nintendo DS has the up key on the D-pad held (which included the
initial down press) and accelerate the ship if so. The up key will constantly
read as held, so long as it is held. Reading the input does not affect the keys
register. We'll do similar things for each of the other keys. See if you can
tell what each key does from the code listing below.

```C++
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
```

As you've noticed, having that Ship class made input handling extremely easy.
Our keys will directly affect various properties of the ship as we press them.
This is really amazing, but the true miracle is yet to come.

As for the moon, since we didn't make one class for it, the code came together
a little more messily. We also had to pass two parameters to the
`handleInput()` function to represent the moon portion of the game state
instead of one. If we had wanted to be more clean, we could have even produced
a game state struct that contained all the game state and all operations on it
in one place.

### Creating the Main Game Loop, Again

Let's check back in on our `main()` function now. We need to make some
adjustments to our game loop since we've now added the ability to collect input
and to react to input. The first thing we now want to happen in our game loop
is for the key registers and read touch screen coordinates to get updated. We
make a call to `updateInput()` and it all happens for us. Next, we handle the
input we just received by calling our recently created `handleInput()`
function, passing in our game state so that it can change it for us. Everything
else is as before.

```C++
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

  /*************************************************************************/

  /* Keep track of the touch screen coordinates */
  touchPosition touch;

  /* Make the ship object */
  static const int SHUTTLE_AFFINE_ID = 0;
  SpriteEntry * shipEntry = &oam->oamBuffer[SHUTTLE_AFFINE_ID];
  SpriteRotation * shipRotation = &oam->matrixBuffer[SHUTTLE_AFFINE_ID];
  Ship * ship = new Ship(&spriteInfo[SHUTTLE_AFFINE_ID]);

  /* Make the moon */
  static const int MOON_AFFINE_ID = 1;
  SpriteEntry * moonEntry = &oam->oamBuffer[MOON_AFFINE_ID];
  SpriteInfo * moonInfo = &spriteInfo[MOON_AFFINE_ID];
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
```

### Compiling

Again, we should double check that we have all the proper includes before
compiling.

```C++
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
```

Now we can control our ship with the D-Pad. What fun! The game should now
appear as in [Figure 8.1, “Flying around in the Orange
Shuttle”](#chapter_8_screen_shot). Now if only we had some aliens to kill…


<a name="chapter_8_screen_shot"></a>

#### Figure 8.1. Flying around in the Orange Shuttle

![Splash](images/splash.png)

![Starfield with Moving Orange
Shuttle](images/starfield_with_moving_orange_shuttle.png)
