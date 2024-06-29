# Chapter 9
## What about the sounds?

### A Sound Theory

Sounds are essential in any game. Our little project should be no exception.
Sounds bring life to various elements in the game, such as space ships, weapon
systems, rain, sword clashing, car engines, and so forth.

Many games don't need music. For instance, a game with the programmer's or game
designer's favorite song may annoy many players who have different tastes in
music. Instead, give the player the option to listen to their own music, on
their own audio equipment. Not to mention, music can be costly in data size.

Sound effects on the other hand, are quite useful. A word of warning, however,
a game with bad sounds (scratchy, annoying, too repetitive, etc.) is worse than
a silent game. Take care when engineering sounds for your game.

### The Hardware

The Nintendo DS has amazing sound hardware. We will only be able to scratch the
surface of it, and even still, we won't leave any scratch marks. The Nintendo
DS has 16 channels to play sounds, numbered 0 through 15. Channels 8-15 are
special channels. All channels can play ADPCM or PCM sounds, while 8-13 are the
only channels that can play PSG sounds and 14-15 the only that can generate
noise. We will only be experimenting with PCM sounds.

### Making the Sounds

To play our sounds, we will use the unbelievably impressive homebrew audio
library, maxmod. maxmod is distributed with libnds, so you should already have
it installed. maxmod can play music as well as sound effects, in such formats
as mod, it, s3m, xm, and wav, but we'll just be using it as a wav player to
play our sound effects. For more information and deeper coverage of the
library, visit <http://maxmod.org/>.

### Using the Sounds

The makefile I've included with my manual has a custom rule for creating a
`soundbank.bin` file from all audio files in the `audio` project folder. The
rule runs mmutil to create the `soundbank.bin` and `soundbank.h` files. The
bin2o rule turns the `.bin` file into something safe to link into your `.nds`
file and creates the `soundbank_bin.h` file. All of these header files will be
located in the `build` folder after a build, if you wish to review their
contents.

### Getting Down with the Code

Now it's finally time for some code. maxmod simplifies sound playing quite a
bit. All we need to do is initialize the library, let it know where our
soundbank is, load the sound from our soundbank, and call a function to play
the sound. It will serve our purposes for now, as a springboard into more
advanced forms of sound. Enough with the chatter, here's some code for our
`main.cpp`.

```C++
void handleInput(Ship * ship,
         MathVector2D<int> * moonPos,
         SpriteInfo * moonInfo,
         touchPosition * touch) {

  /* Handle up and down parts of D-Pad. */
  if (keysDown() & KEY_UP) {
    // Play our sound only when the button is initially pressed
    mmEffect(SFX_THRUST);
  }
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

  /*  Set up sound data.*/
  mmLoadEffect(SFX_THRUST);

  for (;;) {
    /* Update the game state. */
    updateInput(&touch);
    handleInput(ship, moonPos, moonInfo,&touch);
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

In summary, we simply set up our sound in the `main()` function, and had the
`handleInput()` function play our sound whenever the up key is initially
pressed.

### The all powerful maxmod

We are barely touching what maxmod can do in this case study. maxmod can stop
sounds after they've started playing, loop sounds, play music, and more without
breaking a sweat (because the Nintendo DS doesn't have sweat glands anyway). I
highly recommend you read about it on [its homepage](http://maxmod.org/), or at
least run the maxmod demo (featured on said homepage) on a real Nintendo DS.
You won't be disappointed.

### Compiling

Check your includes.

```C++
#include <nds.h>
#include <maxmod9.h>
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
/* Sounds */
#include "soundbank.h"
#include "soundbank_bin.h"
```

This is the final iteration of the Orange Spaceship demo that we will cover in
this edition of the manual. Compile it and enjoy the fruits of your labors.
Mmm, tasty. You should hear a nice sound when you press the thrust button. The
game output should now look like the screen shots in [Figure 9.1, “Flying
around in the Orange Shuttle, with sound!”](#chapter_9_screen_shot).


<a name="chapter_9_screen_shot"></a>

#### Figure 9.1. Flying around in the Orange Shuttle, with sound!

![Splash](images/splash.png)

![Starfied with Moving Orange
Shuttle](images/starfield_with_moving_orange_shuttle.png)
