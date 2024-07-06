// ANCHOR: move_sprite
// Move a Sprite
void moveSprite(SpriteEntry* spriteEntry, u16 x, u16 y) {
  spriteEntry->attribute[1] &= 0xFE00;
  spriteEntry->attribute[1] |= (x & 0x01FF);
  spriteEntry->attribute[0] &= 0xFF00;
  spriteEntry->attribute[0] |= (y & 0x00FF);
}
// ANCHOR_END: move_sprite

// ANCHOR: sprite_priority
void setSpritePriority(SpriteEntry* spriteEntry, int priority) {
  spriteEntry->attribute[2] &= ~ATTR2_PRIORITY_MASK;
  spriteEntry->attribute[2] |= ATTR2_PRIORITY(priority);
}
// ANCHOR_END: sprite_priority
