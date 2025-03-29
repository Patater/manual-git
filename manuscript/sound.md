# Chapter 8
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
{{#include snippets/sound/1/main.cpp:sound}}
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
{{#include snippets/sound/1/main.cpp:headers}}
```

This is the final iteration of the Orange Spaceship demo that we will cover in
this edition of the manual. Compile it and enjoy the fruits of your labors.
Mmm, tasty. You should hear a nice sound when you press the thrust button. The
game output should now look like the screen shots in [Figure 8.1, “Flying
around in the Orange Shuttle, with sound!”](#chapter_8_screen_shot).


<a name="chapter_8_screen_shot"></a>

#### Figure 8.1. Flying around in the Orange Shuttle, with sound!

![Splash](images/splash.png)

![Starfied with Moving Orange
Shuttle](images/starfield_with_moving_orange_shuttle.png)
