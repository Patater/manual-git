# Chapter 4
## How do I create programs?

### All About devkitPro

devkitPro is a brand, like Microsoft or Adobe. You don't use Nintendo DS
software anymore than you write your letters with Microsoft or edit photos with
Adobe. devkitPro produces a collection of toolchains for homebrew developers.
Toolchains are available for Game Boy Advance, GP32, Playstation Portable,
GameCube, and the Nintendo DS. The toolchain we are most interested in is known
as devkitARM.

devkitARM is a specific toolchain of devkitPro. It allows the compiling of ARM
binaries from most all computers. It is based on gcc, the GNU Compiler
Collection. devkitARM includes everything you'll need to create software for
the Nintendo DS, GBA, and GP32; all of which are run by the ARM processor.
However, we will be using something to make our job much easier in addition to
just devkitARM.

### The Wonderful World of libnds

libnds, the library for Nintendo DS, started out its life as NDSLIB. NDSLIB was
a simple library created by joat (Michael Noland) and dovoto (Jason Rogers).
The name was changed to libnds over the course of a few months and the
maintainer has been changed to WinterMute (Dave Murphy).

NDSLIB started out as a collection of defines for common memory locations in
the DS. This is useful, as you can simply reference `BG_BMP_RAM` instead of
`0x06000000`. Eventually, the library began to include structs and unions and
other useful constructs that help to simplify the programmers job and abstract
certain portions of the hardware from the programmer.

Today, libnds is an incredibly useful library that over 96% of the Nintendo DS
homebrew community uses.

### Installing devkitARM

Installing devkitARM is quite simple. Directions are already documented on
their website. Visit <http://www.devkitpro.org/setup.shtml> for directions.
Although more geared towards Windows, the installation is fairly straight
forward. Automated installers are available for Windows, Macintosh, and Linux.

### Installing libnds from source

libnds's source install is less documented than devkitPro's source install, but
is also quite simple in comparison. libnds is automatically installed by the
automated installers of devkitARM. However, if you want to view the libnds
source code, you'll have to install it from source.

#### Procedure 4.1. To install libnds from source

1. Simply download the latest source from
   [SourceForge.net](http://sourceforge.net/project/showfiles.php%3Fgroup_id%3D114505%26package_id%3D151608).
1. Extract it to `$DEVKITPRO/libnds`.
   ```sh
   patater@patater.com:~$ mkdir $DEVKITPRO/libnds
   patater@patater.com:~$ mv libnds-src-*.tar $DEVKITPRO/libnds/
   patater@patater.com:~$ cd $DEVKITPRO/libnds
   patater@patater.com:~$ tar -xvjf libnds-src-*.tar.bz2 $DEVKITPRO/libnds
   ```
1. Change your current directory to `$DEVKITPRO/libnds` and type `make`.
   ```sh
   patater@patater.com:~$ cd $DEVKITPRO/libnds
   patater@patater.com:~$ make
   ```
1. If devkitARM is installed properly, libnds will compile in a matter of
   seconds and you'll be on your way to developing software for the Nintendo
   DS.

### The Next Step

Now that you have devkitARM and libnds installed on your computer, you have
everything you need to start coding, excepting perhaps a bit of knowledge on
how to code specifically for the unique hardware of the Nintendo DS. In the
next chapter, we'll cover the basics of displaying a bitmap on the screen.

![](./images/devkitlogo.png)
