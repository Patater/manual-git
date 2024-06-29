# Chapter 7
## Basic Game Mechanics Applied to the Space Shooter Genre

### The Importance of Object Oriented Programming

Object oriented programming (OOP) is essential to making good games on a modern
system. Although it is very much possible without object oriented programming,
OOP is an incredible tool that greatly improves code reusability, readability,
modularization, and abstraction. It makes the programmer's job a lot easier.
Also, due to modularization, collaborating on projects with your friends or
coworkers is easily ten fold easier.

### The Ship Class

The first thing we'll make is a `Ship` class. This class will encapsulate all
the properties and functionality of any ship in an easy to use and understand
format. Think of things a ship can do, on a high level. What should come to
mind is the ability to turn both ways, shoot weapons, accelerate, move at a
given velocity (coasting), and maybe some more things if you are creative
enough. What properties of a ship can you come up with? Perhaps turning speed,
thrust, mass, maximum speed, velocity, position, shields? Well, after you are
done brainstorming, the next step is to write out the functionality and
properties we need to put into our `Ship` class. You could make a table, as in
[Table 7.1, “Table of Ship properties and
functionality”](#ship_properties_and_functions), or draw some diagrams on a
piece of paper. Either way, you want to make sure your ideas all get onto some
physical paper before you begin coding.

<a name="ship_properties_and_functions"></a>

#### Table of Ship properties and functionality

| Properties | Functionality |
| ---------- | ------------- |
| `shipHeight` | `accelerate` |
| `shipWidth` | `moveShip` |
| `position` | `turnClockwise` |
| `velocity` | `turnCounterClockwise` |
| `angle` | `getPosition` |
| `turnSpeed` | `reverseTurn` |
| `thrust` | `getAngle` |
| `maxSpeed` | |
| `mass` | |

### Making the Ship Class

I have provided a skeleton framework file for you to write your class in. It is
all set and ready for you to implement in the `ship.cpp` file. The header file,
`ship.h` is also included. On your own, with your own classes in the future,
you should always make a skeleton framework class to work from. It makes
implementation straightforward and you do not have to worry about the semantics
of setting up a class so much.

### The Constructor

I have provided you with a simple constructor and private initialization method
method. These are often mundane things to make. In fact, the compiler will
automatically make the constructor, copy constructor, and operator= methods for
you if you don't explicitly make them. Feel free to modify the default values
in the initializers to try out different effects of changing the ship
properties.

### Acceleration

Acceleration is probably one of the most important things your ships can do. To
accelerate, we simply increase our velocity by a certain increment, that being
the thrust capability of the ship, in the angle we are headed. Here is where
some simple trigonometry comes into play. Since our velocity is stored as a two
dimensional vector (x and y component), we have to shadow our thrust vector
onto each direction. We do this we multiply the thrust by sin(angle) for our x
component, and by -cos(angle) for the y direction. Next, after we have computed
the increment for both x and y, we add them onto our current velocity, making
sure we don’t go over the ship's maximum speed.

```C++
void Ship::accelerate() {
  float incX = thrust * sin(angle);
  float incY = -(thrust * cos(angle));

  //the following method of speed limitation is not accurate, traveling
  //diagonally is faster than straight, which is not the desired limitation
  //a more accurate method is needed at a later time

  velocity.x += incX;
  //make sure can't go too fast in x direction
  if (velocity.x > maxSpeed) {
    velocity.x = maxSpeed;
  }
  if (velocity.x < -maxSpeed) {
    velocity.x = -maxSpeed;
  }

  velocity.y += incY;
  //make sure can't go too fast in y direction
  if (velocity.y > maxSpeed) {
    velocity.y = maxSpeed;
  }
  if (velocity.y < -maxSpeed) {
    velocity.y = -maxSpeed;
  }
}
```

### Moving the Ship

This one is incredibly easy thanks to the Nintendo DS hardware. All we have to
do is increment our position by our velocity. The hardware takes care of any
wrapping or offscreen issues.

```C++
void Ship::moveShip() {
  //move the ship
  position.x += velocity.x;
  position.y += velocity.y;

  //hw does wrap around for us, so we don't have to have any of that sort of
  //logic in here
}
```

### Reversing the Ship's Direction

This one took me a while to figure out, even though it's just one line, but
it's very useful. We can turn the ship around, not a 180 per se, but simply
pointing into the opposite direction of our current velocity. This will get the
angle of our velocity with respect to 0 degrees, and then will do a 180 from
that angle.

```C++
void Ship::reverseTurn() {
  angle = (2 * PI) - atan2(velocity.x, velocity.y);
}
```

### Rotating the Ship

Rotating the ship is also quite simple. We just increment or by ship’s turning
speed depending on which direction we wish to turn. Note that we are storing
the angle of the ship as a clockwise rotation. This will be important to
remember for later when we update the ship sprite.

```C++
void Ship::turnClockwise() {
    angle += turnSpeed;
}
void Ship::turnCounterClockwise() {
    angle -= turnSpeed;
}
```

### Getting the Ship's Position

Return the ship’s position.

```C++
MathVector2D<float> Ship::getPosition() {
    return position;
}
```

### Getting the Ship's Angle

This one is a bit more tricky and involved. I suppose I should start by
explaining that a Nintendo DS circle has 32768 degrees. It doesn't actually
have 32768 degrees, nor does a Nintendo DS even know what a circle is, but it
is easy to understand the hardware a bit better when we think of it this way. I
will say, however, that the reason for the 32768 degrees is due to libnds's
built-in look up tables for the sin and cos functions. Having only 360 entries
in your lookup table would be a waste of space when it takes just as many bits
to index into a 360 entry table as it does a 512 entry one. libnds gives us
more accuracy by presenting a 32768 entry one. More entries allow finer
accuracy. In order for the Nintendo DS to know how to rotate our sprites, we
have to convert the internally stored radian angle value to a value within the
32768 degree system. This is an easy conversion.

The first step is to convert to a 360 degree system, as you must have learned
in junior high school. This is done by multiplying the radian value by 180/PI.
The 180 part is half the number of degrees in a circle. So, in a 32768 degree
system we can convert by multiplying the radian value by
`DEGREES_IN_A_CIRCLE/(2 * PI)`. Lastly, just return that value as an integer.
(The hardware does not have any floating point, so when rotating our sprites,
we use a fixed point value disguised as an ordinary integer.)

Then, we make a function to return a converted angle value, for whenever we
need it.

```C++
int Ship::radToDeg(float rad) {
    return (int)(rad * (DEGREES_IN_A_CIRCLE/(2 * PI)));
}
int Ship::getAngleDeg() {
    return radToDeg(angle);
}
```

### Linking the Ship into our Program

We now need to create an instance of the ship in our main function. Creating an
instance of a class, known as an object, is quite simple, as you can see below.
We just have to create the Ship object and then assign a `SpriteEntry` to it.

We should also do something nifty with our new class so that we can verify that
what we wrote is working. Let's make the ship move around on its own by telling
the ship to trust ten times.

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

  /* Make the ship object */
  static const int SHUTTLE_OAM_ID = 0;
  SpriteEntry * shipEntry = &oam->oamBuffer[SHUTTLE_OAM_ID];
  SpriteRotation * shipRotation = &oam->matrixBuffer[SHUTTLE_OAM_ID];
  Ship * ship = new Ship(&spriteInfo[SHUTTLE_OAM_ID]);

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

  return 0;
}
```

### Creating the Main Game Loop

The previous code isn't very exciting, since we never update the OAM more than
once. We need to begin the creation of what is referred to as the game loop. We
won't be fully implementing it in this chapter, since a major component of it
will be missing until we discover input on the Nintendo DS.

The game loop has at least three major components. The first thing any game
loop should do is to collect input from the outside world. We won't be doing
that in this chapter, however. The next component of the game loop is updating
the game state. Based on inputs the game received in the previous frame (to the
one we'll render next) and the passing of time, the game state will change (if
anything interesting is happening). The final component of the game loop is the
rendering component. In our case, we have to update the OAM to let it know of
the changes that occurred in the game state and that it needs to reflect those
changes.

Now that we know what a game loop is, it's time for us to start creating one to
run our program. The first thing we want to happen in our game loop is for the
game state to be updated. This is because we don't have any input to collect
yet. We tell our ship to move at it's current velocity. This will change the
ship’s position. Then we update the sprite attributes with new information
about our ship, as some properties of the ship have now changed (i.e. its angle
and position). Note that the `rotateSprite()` function performs a
counter-clockwise rotation and our ship tracks its angle as a clockwise
rotation; this is resolved by simply negating the angle to rotate by. Finally,
we call a function that will make sure our program does not exceed 60fps (speed
of the graphics on the Nintendo DS) by waiting for VBlank, and then we update
the OAM, telling it that we changed some attributes on the sprites and it needs
to handle that.

```C++
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
```

The OAM really shines through here. The all powerful Nintendo DS hardware, an
incredible masterpiece, will rotate and move our ship with very little effort
on our part. In hindsight, all we have done is flip a few bits in a few
registers in a structured manner, and our ship comes to life. Incredible.

### Compiling

Verify that you are including all the files you need to include now, before
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

Everything should compile for you fine at this point if you wish to play around
with your new class. However, in the next chapter we will cover how to get
Nintendo DS input to affect the Ship. Be ready for it, we're going to have some
major fun.

![Orange Shuttle](images/orange_shuttle.png)
