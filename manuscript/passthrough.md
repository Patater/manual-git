# Appendix C
## What is a passthrough device and how do I use one?

### Purpose of the Passthrough

The purpose of the pass through is to allow the running of programs on the
Nintendo DS through the Game Boy Advance (GBA) cartridge slot. This is done
because normal Nintendo DS games, which run from the Nintendo DS (NDS) card
slot, are encrypted. Since it might be illegal to break that encryption, as it
is a form of proprietary copy protection, we have to get the Nintendo DS to run
code from a different place than the NDS card slot. Also, it is much easier to
bypass the encryption than to try and break it.

This kind of device was popular at the beginning of the scene of the Nintendo
DS. Nowadays, they have been replaced by devices that are inserted in the DS
slot of the console. While it is still possible to use passthrough devices, they
are mostly a relic from the past.

### How a PassMe Works

When the Nintendo DS first boots, it reads a header from the Nintendo DS card
inserted into the NDS card slot. The Nintendo DS will read a small block of
information from the beginning of the game, known as the header, which contains
information about the game (including the title of the game, date it was
released and by who, the icon, and some hardware information). This header
contains a pointer to a location in memory to begin executing code. The
passthrough's job is to read this header and modify it to point to a location
someplace on the GBA cartridge inserted into the GBA cart slot. What location
would this be? Why, our code of course.

### History of the Passthrough

DarkFader (Rafael Vuijk) may have been the first to create a passthrough
device. He designed his passthrough with an FPGA (Field Programmable Gate
Array) and later packaged it into a single CPLD. Many other people (including
Kraln, Firefly, and Natrium42) have developed a passthrough independently from
DarkFader, but he is self-credited as its initial creator.

Over the next few months following the creation of the passthrough, Lynx and
Natrium42, both major players in the early homebrew community, started their
own online stores selling pre-made passthrough devices called "PassMe"s. Many
budding DS programmers bought their first passthrough from Lynx or Natrium42's
stores (myself included). Lynx's store, DSPassme.com, is still up and running
today, providing a great option when looking to purchase a passthrough device.


<a name="early_passme"></a>

#### Figure C.1. Picture of an early PassMe and an FPGA

![Picture of an early PassMe](./images/passme.png) ![Picture of an FPGA](./images/ufb-x_profil.jpg)

### The Future is Now, and Now, and Now, and Now

The PassMe was the first device invented to allow homebrew software to be run
on the Nintendo DS. After Nintendo decided to crack down a little harder, in
efforts to prevent piracy, these devices no longer worked on new Nintendo DS
systems. The community then developed the PassMe 2. This device was more fancy
in how it bypassed the copy protection of the Nintendo DS and also required the
operator to have more resources (a GBA flash cart with SRAM) and go through a
more involved process. After a while, a device called the NoPass surfaced.

### About the NoPass

A NoPass pretends to be a real Nintendo DS game card. It contains code that is
encrypted in the same manner as a real Nintendo DS game. The encryption was
broken by Martin Korth, who discovered a way to dump the Nintendo DS BIOS. He
then reverse engineered the encryption information from the BIOS code. NoPass
devices are technically not considered passthroughs, since they don't bypass
the copy protection of the Nintendo DS.

### About Slot-1 Devices

Recently, due to Martin Korth's work, homebrew engineers have discovered a way
to create a device that runs homebrew software from the Nintendo DS card slot
(Slot-1). These new devices contain small passthrough devices (of the NoPass
style) inside them that allow them to bypass copy protection as well some means
of providing memory to the Nintendo DS (via built-in flash memory or a microSD
card slot). These devices are called "Slot-1 devices". The older style devices
that plug into the GBA cart slot are referred to as "Slot-2 devices".

### How do I get a Passthrough

Today, many passthrough and NoPass devices are on the market. It is highly
recommended by the homebrew community to buy one from a fellow developer and
not a company that commercially produces hardware meant for pirating Nintendo
DS software. Thus, one should not buy a SuperPass or SuperKey from SuperCard, a
MagicKey or MK4-Mini from NeoFlash, a PassKey from G6Flash, a PassKey or
Passcard from M3Adapter, or a Max Media Launcher from the horrid Datel (a very
unprofessional company run by immature anti-Nintendo kiddies). Each of these
companies manufactures hardware that is used to pirate Nintendo DS software and
should be avoided at all costs. The best place to buy a passthrough device
today is from
[DSPassme.com](http://www.dspassme.com/oscommerce/catalog/index.php).

### Which Passthrough Should I Buy?

It is recommended that you purchase a NoPass or a Slot-1 homebrew device, as
these work on all Nintendo DS systems with no troubles or other complications
whatsoever. The older passthroughs, like PassMe, don't work on all Nintendo DS
systems, and the PassMe 2 requires extra work and resources to use, not to
mention they both stick out of your Nintendo DS like a sore thumb and are hard
to keep in your pocket.

Slot-1 devices also have many other advantages over Slot-2 devices and
passthroughs. You definitely want to get a Slot-1 device. For instance, Slot-2
devices suck up a lot more power than Slot-1 devices do. This means that using
a Slot-1 device will give you more battery life than using a Slot-2 device.
Many Slot-1 devices also double as NoPass devices, so you don't need to buy
both a passthrough and a Slot-2 device anymore.

### How would I choose an old style passthrough?

If you still want to buy an old fashioned style passthrough, there are a number
of things you'll need to consider. Depending on what kind of Nintendo DS you
have, you can buy either of two different types of passthrough devices. The
first one, akin to the original made by DarkFader, is the "PassMe". The PassMe
will probably work on most early "phat" (non-Nintendo DS Lite) Nintendo DS
systems.

However, if your Nintendo DS is newer (both phat and Lite systems can be new),
you will most likely need a "PassMe 2". The need for a PassMe 2 came about
because of a change in the firmware of newer Nintendo DS systems. These newer
systems do not allow the header to be changed to point to code in the GBA slot.
However, it is possible to point to code in the GBA carts SRAM still. The
PassMe 2 points to some certain instructions that have been loaded into memory
by the currently inserted DS game. This location is an SWI (software interrupt)
call to the SRAM on the GBA port. The GBA carts SRAM contains the necessary
code to jump to code located on GBA cart. The DS then is told to run this code
and thus redirects to code in the GBA slot. Each PassMe 2 has to be programmed
to work with one specific game, as the certain code to redirect to the GBA slot
is found in different places within different DS games.

There are a few tricks you can pull to determine your firmware version. It
involves pictochat and pulling a cartridge out of your Nintendo DS. The screen
will change a certain color. Based on this color you can determine if you need
a PassMe 2 or not. <a href="#game_eject_firmware_table">Table C.1, “When
ejecting the game, you'll discover your firmware version as shown."</a> will
help you discover which firmware version you have.

#### Procedure C.1. Checking the Firmware Version

1. Insert and Nintendo DS game into the DS card slot.
1. Turn on the Nintendo DS. (If you have auto start enabled, hold down Start
   and Select when you boot the system.)
1. Start Pictochat by pressing the Pictochat logo on the menu screen.
1. Join any room you wish.
1. Remove the Nintendo DS game inserted into the back of the system.
1. You will notice either Pictochat freezing or both screens will
   change color.

#### Table C.1. When ejecting the game, you'll discover your firmware version as shown.

<a name="game_eject_firmware_table"></a>
| Observed Results | Firmware Version |
| ---------------- | ---------------- |
| Pictochat froze  | Version 1 |
| Both screens turned periwinkle | Version 2 |
| Both screens turned dark green | Version 3, iQue, or Flashme |
| Both screens turned marigold | Version 4 |
| Both screens turned magenta | Version 5 |
| Both screens turned dark blue | Version 6 |

It is recommended that you purchase a NoPass instead of a PassMe or PassMe 2
passthrough device if you are unsure which to get, as they are guaranteed to
work with all Nintendo DS systems, past, present, and future. If you have a
friend with a newer DS who may need to borrow your NoPass to play your games,
it's always helpful to have a universal method of running them.

### PassMe 2 Buying Tips

When ordering a PassMe 2, be sure to select one pre-programmed to a common game
that you own, such as the Metroid Prime Hunters Demo or Super Mario 64 DS. Each
PassMe 2 must be programmed to a specific game and it is best to select a game
that is in large supply. Also, you'll need a GBA flash cart with some SRAM on
it. Most GBA flash carts have SRAM, but be sure to check your cart before
trying to obtain a PassMe 2. Some newer GBA flash carts might only support save
games on flash, so simply knowing that your flash cart supports save games
isn't enough.

### How do I use my Passthrough

Using a passthrough device is quite simple. Simply insert a game into the card
slot on the passthrough and plug the device into the DS card slot on the top of
your DS, like any other game. <a href="#passme_in_use">Figure C.2, “The PassMe
inserted into the DS card slot”</a> illustrates what this looks like.


<a name="passme_in_use"></a>

#### Figure C.2. The PassMe inserted into the DS card slot

![The passme inserted into the DS card slot](./images/passme_insert_ds.png)

### What to do with your Passthrough

One of the best things to do once you get your passthrough is to install a
patch to your Nintendo DS firmware known as "FlashMe." This firmware patch
allows for the running of unsigned wifi binaries as well as the redirecting of
the Nintendo DS to boot from the GBA slot in NDS mode. This means that after
installing FlashMe, you no longer need your PassMe. FlashMe is the preferred
and recommended method of running your own software. You no longer have to lug
around a passthrough or play with a large block sticking out of the back of
your DS. Also, you can send your code via wireless multiboot (WMB), if you so
desire, and avoid having to use a flash cart. In addition to all those things,
in case you ever come across some malicious code that zaps your firmware or
otherwise messes up your DS, FlashMe keeps a recovery program in the protected
firmware space that you can run to save your DS. Because of this feature alone,
FlashMe is better than the standard firmware. FlashMe can be obtained from
<http://home.comcast.net/~olimar/flashme/>.
