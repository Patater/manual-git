void demo(void) {
  // ANCHOR: set_up_bg
  BG2_CR =
      BG_BMP16_256x256 | BG_BMP_BASE(0) | BG_PRIORITY(3);  // on the bottom
  BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(8) | BG_PRIORITY(0);  // on the top
  // ANCHOR_END: set_up_bg
}
