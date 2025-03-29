# Chapter 2
## How do I get programs into my Nintendo DS?

### The Methods

There are a few ways of getting your code into the Nintendo DS or DSi. The
preferred method depends on which device you own. You will need a flash cart
device if you own a Nintendo DS, but you don't need more than a regular SD card
if you own a Nintendo DSi.

Flash carts are devices that have the shape of a regular DS game, except that
they contain a microSD slot for you to add your own files. There are plenty of
slot-1 devices, and all of them work in regular Nintendo DS consoles.

There is also the option of using slot-2 devices, but they are hard to find, and
a bit awkward to use. Check [Appendix C](passthrough.md) for more information
about how they work.

If you own a Nintendo DSi it's a bit different. You can use slot-1 devices, but
only the most modern ones. Nintendo DS consoles don't have very advanced
protection systems, but Nintendo DSi consoles are much better at detecting
unauthorized devices. Only a few slot-1 devices are still supported.

However, slot-1 devices aren't needed if you own a Nintendo DSi! There are ways
to modify your console using software to load unauthorized code from the SD card
slot of the console. This means that you don't need to buy anything at all!
There are guides online that teach you how to do this.

### Which Slot-1 Device should I buy?

As there are no homebrew produced devices made by homebrew people for homebrew
people (yet), we regretfully have to buy devices designed and sold for piracy
when buying NoPass or Slot-1 devices. That said, the most important feature to
consider when buying one of these devices is DLDI support.

### What is DLDI?

DLDI stands for dynamically linked device interface. In order for the homebrew
filesystem, known as libfat, to work, you'll need a device which supports the
DLDI standard. DLDI specifies a system by which DLDI modules can be dynamically
loaded into homebrew software. This allows developers to release one version of
their homebrew which the software end users simply patch and then run on a
device of their choice. If you buy a device that does not support DLDI, you
will be out of luck when wanted to run a majority of the homebrew applications
and games out there. Not to mention that the company producing the device is
essentially saying, "We don't need to spend time catering to the homebrew
community since our device is designed just for piracy." We'd be better off not
supporting that kind of company with our business.

You can learn more about DLDI and how to use it at the DLDI Wiki website,
<http://dldi.drunkencoders.com/index.php?title=Main_Page>.

### Where do I get one of these Slot-1 devices?</title>

Out of the many places to buy these devices, I've been most happy with
electrobee. electrobee is run by a trusted member of the homebrew community and
ships worldwide from good ol' Canada (as opposed to who knows where). Their
prices are quite often the best, too.

### The Slot-2 Device of Choice

If you decide that GBA software development is important for you, you might
want to consider getting a NoPass and a Slot-2 device. The NoPass will allow
you to run Nintendo DS software from your Slot-2 device. The Slot-2 device, on
its own, will be able to run GBA software. If you only care about Nintendo DS
software, a Slot-1 device will meet your needs well, and you can skip the rest
of this chapter.

The GBA Movie Player is a wonderful device which can run your software from a
Compact Flash card. Compact Flash cards are very cheap and in high supply. If
you have more SD cards than CF cards around, unfortunately, the only way to run
NDS software from an SD card at this time is with the SD versions of the
SuperCard or M3 Adapter. The GBAMP also sticks out from the bottom of the
Nintendo DS a little, as shown in [Figure 2.1, “Comparison of GBAMP (left) and
a GBA flash cart (right) inserted into a DS”](#gbamp_vs_gba_flash_cart).


<a name="gbamp_vs_gba_flash_cart"></a>

#### Figure 2.1. Comparison of GBAMP (left) and a GBA flash cart (right) inserted into a DS

![Comparison of a GBAMP (left) and a GBA flash cart (right) inserted into a
DS](./images/comparison_insertion.png)

With the GBA flash cart, the process of loading your programs into memory is a
bit slow. Also, each GBA flash cart writer is specific to certain carts and
often have closed source drivers. This means that most flash carts will be
incompatible with Linux or Macintosh computers. In actuality, the only good
thing about a GBA flash cart is that it does not stick out from the Nintendo
DS, as can be seen in [Figure 2.1, “Comparison of GBAMP (left) and a GBA flash
cart (right) inserted into a DS”](#gbamp_vs_gba_flash_cart).


### Running Multiple Software Titles

If you chose to go with the GBA flash cart, you can use a utility called
Darkain’s MultiNDS Loader to load multiple programs onto your flash cart. This
way, you don’t have to re-flash your cart each time you want to run a different
application.

If you chose to go with the GBAMP, then I’d highly recommend DragonMinded's
DSOrganize. It supports booting multiple programs, text editing, address books,
calendars, and more. It's a great application for the DS. You can get it from
<http://www.dragonminded.com/?loc=ndsdev/DSOrganize>. However, you cannot use a
stock GBAMP to run NDS programs. You have to flash it with some custom
firmware. Instructions and firmware are available from
<http://chishm.drunkencoders.com/NDSMP/index.html>.

As for Slot-1 devices, these all usually come with their own menu systems
(often incorrectly referred to as operating systems). You won't have to do
anything special to run multiple software titles.
