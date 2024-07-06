# Appendix A
## VRAM

### Overview

I compiled these tables so that it would be easy to see what banks of VRAM
could be used for which purposes. I hope this will be of help to those of you
designing your 2D graphic engines or are wanting a more in depth coverage of
the Nintendo DS VRAM. I hope they will be as useful to you as they continue to
be for me.

Usually, using this appendix is just a two step process. Based on what you want
to do, use the usage tables to figure out which VRAM banks are suited to the
task. Next, check the layout tables to see if there exist any conflicts between
bank memory locations and assignments. If there are no conflicts, you have
successfully come up with a VRAM layout for what you want to do. Otherwise,
simply go back to the usage charts to see if there are any alternative ways of
doing what you want to do. If what you want to do still seems impossible,
consider using the 3D hardware.

## VRAM Usage

### Notes and Precautions

In the tables, you’ll see some numbers. These numbers are called “usage index
numbers.” The best usage index number is one: it matches perfectly for that
particular task. Greater than one means a wasteful design. Less than one means
that you won’t be able to complete the whole task. Individual VRAM banks may
have a rating of less than one for any particular task, but when used in
conjunction with another bank, those two banks both might be perfectly suited
for that task.

For example, if we wanted to store tiles for 128 unique 16 color sprites, the
best choice is to use two banks that each have a rating of less than one (.5),
F and G. These two banks together sum to one, a perfect match for the task.

There are exceptions to the one is best rule, however. Keep in mind that if you
want to double a task, as in you want two 16-bit color bitmaps for instance,
you’ll want to sum your usage index number to two. Likewise, if you wanted 10
256 color external backgrounds (for whatever crazy reasons you may have),
you’ll want to sum your usage index number to ten. Also note that if assigning
leftover banks to auxiliary memory (LCD or Work RAM), a larger number simply
gives you more memory, so the one is best rule doesn’t really apply for that
table’s purposes.

Each bitmap in the table has a size of 256x256 and is at the specified color
depth. If you need a larger bitmap, do the appropriate math to linearly
transform the usage index numbers. For instance, a 512x512 16-bit color bitmap
will take up 4 times as much memory as a 256x256 sized one. Thus, we multiply
our usage numbers by .25 for that particular row, the 16-bit color bitmap row.
We then see that it will take VRAM banks A, B, C, and D to meet our purpose.
Likewise, if we wanted to do a smaller bitmap, we would multiply by a number
larger than 1 for that particular row.

Just because the usage charts say that it is possible, make sure that you check
the layout charts to be sure that using your specific combination of the banks
is legal.


### Main Screen Usage

| VRAM Bank | A | B | C | D | E | F | G | H | I |
| --------- | - | - | - | - | - | - | - | - | - |
| **16-Bit Color Bitmap** | 1 | 1 | 1 | 1 | 0.5 | 0.125 | 0.125 | N/A | N/A |
| **256 Color Paletted Bitmap** | 2.66 | 2.66 | 2.66 | 2.66 | 1.33 | 0.66 | 0.66 | N/A | N/A |
| **128 Unique 256 Color Sprites** | 2 | 2 | N/A | N/A | 1 | 0.25 | 0.25 | N/A | N/A |
| **128 Unique 16 Color Sprites** | 4 | 4 | N/A | N/A | 2 | 0.5 | 0.5 | N/A | N/A |
| **Background Extended Palette** | N/A | N/A | N/A | N/A | 1 | 0.5 | 0.5 | N/A | N/A |
| **Object Extended Palette** | N/A | N/A | N/A | N/A | N/A | 1 | 1 | N/A | N/A |


### Sub Screen Usage

| VRAM Bank | A | B | C | D | E | F | G | H | I |
| --------- | - | - | - | - | - | - | - | - | - |
| **16-Bit Color Bitmap** | N/A | N/A | 1 | N/A | N/A | N/A | N/A | 0.25 | 0.125 |
| **256 Color Paletted Bitmap** | N/A | N/A | 2.66 | N/A | N/A | N/A | N/A | 0.66 | 0.25 |
| **128 Unique 256 Color Sprites** | N/A | N/A | N/A | 2 | N/A | N/A | N/A | N/A | 0.25 |
| **128 Unique 16 Color Sprites** | N/A | N/A | N/A | 4 | N/A | N/A | N/A | N/A | 0.5 |
| **Background Extended Palette** | N/A | N/A | N/A | N/A | N/A | N/A | N/A | 1 | N/A |
| **Object Extended Palette** | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | 1 |


### Auxiliary Memory Usage

| VRAM Bank | A | B | C | D | E | F | G | H | I |
| --------- | - | - | - | - | - | - | - | - | - |
| **LCD (ARM9)** | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| **Work RAM (Arm7)** | N/A | N/A | 1 | 1 | N/A | N/A | N/A | N/A | N/A |


### Texture Usage

| VRAM Bank | A | B | C | D | E | F | G | H | I |
| --------- | - | - | - | - | - | - | - | - | - |
| Texture Image | 1 | 1 | 1 | 1 | N/A | N/A | N/A | N/A | N/A |
| Texture Palette | N/A | N/A | N/A | N/A | 1 | 0.25 | 0.25 | N/A | N/A |


## VRAM Layout

### Notes and Precautions

To use the layout charts, look for the VRAM bank you want to map and select a
memory location to map it to. Depending on what you want to do, you’ll want to
refer to the numbers listed under the usages. These numbers represent the
offset you’ll need to use to access your VRAM bank.

For example, say we decided to use VRAM banks A and B to display a 16-bit color
bitmap. We would look for VRAM banks A and B under the VRAM Banks column on the
“Main Screen Background” table. After finding them, we notice that we can
assign them to memory locations `0x0600:0000`, `0x0602:0000`, `0x0604:0000`,
and `0x0606:0000`. We then choose to map VRAM bank A to `0x0600:0000` and VRAM
bank B to `0x0602:0000`. To access our 16-bit color bitmaps we use the offset
of 0 for the first and 8 for the second. In the case of backgrounds, libnds
provides the `BG_BMP_BASE` macro for use to use when setting up our
backgrounds. In this case, some code we might use to set up `BG2` and `BG3`
might look like the following.

```C++
{{#include snippets/vram/1/main.cpp:set_up_bg}}
```

Slots are not readable or writable by the ARM9 or the ARM7. No usage is
necessarily implied on these tables, only layout in comparison to the sizes of
relevant data.

Each bitmap in the table has a size of 256x256 and is at the specified color
depth.


### Main Screen Background

| Memory Location | 16-Bit Color Bitmap | 256 Color Paletted Bitmap | VRAM Banks |
| --------------- | ------------------- | ------------------------- | ---------- |
| **`0x0600:0000`** | 0  | 0   | A,B,C,D,E,F,G |
| **`0x0600:4000`** | 0  | 0   | F,G |
| **`0x0600:8000`** | 0  | 0   | F,G |
| **`0x0600:C000`** | 0  | 3   | F,G |
| **`0x0601:0000`** | 0  | 3   | F,G |
| **`0x0601:4000`** | 0  | 3   | F,G |
| **`0x0601:8000`** | 0  | 6   | F,G |
| **`0x0601:C000`** | 0  | 6   | F,G |
| **`0x0602:0000`** | 8  | 6   | A,B,C,D,F,G |
| **`0x0602:4000`** | 8  | 9   | F,G |
| **`0x0602:8000`** | 8  | 9   | F,G |
| **`0x0602:C000`** | 8  | 9   | F,G |
| **`0x0603:0000`** | 8  | 12  | F,G |
| **`0x0603:4000`** | 8  | 12  | F,G |
| **`0x0603:8000`** | 8  | 12  | F,G |
| **`0x0603:C000`** | 8  | 15  | F,G |
| **`0x0604:0000`** | 16 | 15  | A,B,C,D |
| **`0x0604:4000`** | 16 | 15  | |
| **`0x0604:8000`** | 16 | 18  | |
| **`0x0604:C000`** | 16 | 18  | |
| **`0x0605:0000`** | 16 | 18  | |
| **`0x0605:4000`** | 16 | 21  | |
| **`0x0605:8000`** | 16 | 21  | |
| **`0x0605:C000`** | 16 | 21  | |
| **`0x0606:0000`** | 24 | 24  | A,B,C,D |
| **`0x0606:4000`** | 24 | 24  | |
| **`0x0606:8000`** | 24 | 24  | |
| **`0x0606:C000`** | 24 | 28  | |
| **`0x0607:0000`** | 24 | 28  | |
| **`0x0607:4000`** | 24 | 28  | |
| **`0x0607:8000`** | 24 | N/A | |
| **`0x0607:C000`** | 24 | N/A | |


### Main Screen Object

| Memory Location | 128 Unique 256 Color Sprites | 128 Unique 16 Color Sprites | VRAM Banks |
| --------------- | ---------------------------- | --------------------------- | ---------- |
| **`0x0640:0000`** | 0  | 0  | A,B,E,F,G |
| **`0x0640:4000`** | 0  | 0  | F,G |
| **`0x0640:8000`** | 0  | 2  | F,G |
| **`0x0640:C000`** | 0  | 2  | F,G |
| **`0x0641:0000`** | 4  | 4  | F,G |
| **`0x0641:4000`** | 4  | 4  | F,G |
| **`0x0641:8000`** | 4  | 6  | F,G |
| **`0x0641:C000`** | 4  | 6  | F,G |
| **`0x0642:0000`** | 8  | 8  | A,B,F,G |
| **`0x0642:4000`** | 8  | 8  | F,G |
| **`0x0642:8000`** | 8  | 10 | F,G |
| **`0x0642:C000`** | 8  | 10 | F,G |
| **`0x0643:0000`** | 12 | 12 | F,G |
| **`0x0643:4000`** | 12 | 12 | F,G |
| **`0x0643:8000`** | 12 | 14 | F,G |
| **`0x0643:C000`** | 12 | 14 | F,G |


### Main Screen Background Extended Palette

Background extended palette slots are 32KB each, despite which VRAM bank is
assigned to them.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | E,F,G |
| **1** | E,F,G |
| **2** | E,F,G |
| **3** | E,F,G |


### Main Screen Object Extended Palette

Object extended palette slots are 8KB each, despite which VRAM bank is assigned
to them.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | F,G |


### Sub Screen Background

| Memory Location | 16-Bit Color Bitmap | 256 Color Paletted Bitmap | VRAM Banks |
| --------------- | ------------------- | ------------------------- | ---------- |
| **`0x0620:0000`** | 0 | 0   | C,H |
| **`0x0620:4000`** | 0 | 0   | |
| **`0x0620:8000`** | 0 | 0   | I |
| **`0x0620:C000`** | 0 | 3   | |
| **`0x0621:0000`** | 0 | 3   | |
| **`0x0621:4000`** | 0 | 3   | |
| **`0x0621:8000`** | 0 | N/A | |
| **`0x0621:C000`** | 0 | N/A | |


### Sub Screen Object

| Memory Location | 128 Unique 256 Color Sprites | 128 Unique 16 Color Sprites | VRAM Banks |
| --------------- | ---------------------------- | --------------------------- | ---------- |
| **`0x0660:0000`** | 0 | 0 | D,I |
| **`0x0660:4000`** | 0 | 0 | |
| **`0x0660:8000`** | 0 | 2 | |
| **`0x0660:C000`** | 0 | 2 | |
| **`0x0661:0000`** | 4 | 4 | |
| **`0x0661:4000`** | 4 | 4 | |
| **`0x0661:8000`** | 4 | 6 | |
| **`0x0661:C000`** | 4 | 6 | |


### Sub Screen Background Extended Palette

Background extended palette slots are 32KB each, despite which VRAM bank is
assigned to them.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | H |
| **1** | H |
| **2** | H |
| **3** | H |


### Sub Screen Object Extended Palette

Object extended palette slots are 8KB each, despite which VRAM bank is assigned
to them.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | I |


### Texture Palette

Texture palette slots are 64KB each.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | E,F,G |
| **1** | E,F,G |
| **2** | E,F,G |
| **3** | E |
| **4** | F,G |
| **5** | F,G |


### Texture Image

Texture image slots are 128KB each.

| Slot | VRAM Banks |
| ---- | ---------- |
| **0** | A,B,C,D |
| **1** | A,B,C,D |
| **2** | A,B,C,D |
| **3** | A,B,C,D |

### ARM9 Access (LCD)

| Memory Location | VRAM Banks |
| --------------- | ---------- |
| **0x0680:0000** | A |
| **0x0682:0000** | B |
| **0x0684:0000** | C |
| **0x0686:0000** | D |
| **0x0688:0000** | E |
| **0x0689:0000** | F |
| **0x0689:4000** | G |
| **0x0689:8000** | H |
| **0x068A:0000** | I |


### ARM7 Access (Work RAM)

| Memory Location | VRAM Banks |
| --------------- | ---------- |
| **`0x0600:0000`** | C,D |
| **`0x0602:0000`** | C,D |
