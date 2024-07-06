#ifndef DECLARATIONS_H
#define DECLARATIONS_H

// ANCHOR: dmaCopyHalfWords
static inline void
dmaCopyHalfWords(
    uint8 channel,
    const void *source,
    void *dest,
    uint32 size
);
// ANCHOR_END: dmaCopyHalfWords

#endif
