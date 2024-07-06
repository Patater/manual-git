// ANCHOR: accelerate
void Ship::accelerate() {
  float incX = thrust * sin(angle);
  float incY = -(thrust * cos(angle));

  // the following method of speed limitation is not accurate, traveling
  // diagonally is faster than straight, which is not the desired limitation
  // a more accurate method is needed at a later time

  velocity.x += incX;
  // make sure can't go too fast in x direction
  if (velocity.x > maxSpeed) {
    velocity.x = maxSpeed;
  }
  if (velocity.x < -maxSpeed) {
    velocity.x = -maxSpeed;
  }

  velocity.y += incY;
  // make sure can't go too fast in y direction
  if (velocity.y > maxSpeed) {
    velocity.y = maxSpeed;
  }
  if (velocity.y < -maxSpeed) {
    velocity.y = -maxSpeed;
  }
}
// ANCHOR_END: accelerate

// ANCHOR: move
void Ship::moveShip() {
  // move the ship
  position.x += velocity.x;
  position.y += velocity.y;

  // hw does wrap around for us, so we don't have to have any of that sort of
  // logic in here
}
// ANCHOR_END: move

// ANCHOR: reverse_turn
void Ship::reverseTurn() { angle = (2 * PI) - atan2(velocity.x, velocity.y); }
// ANCHOR_END: reverse_turn

// ANCHOR: rotate
void Ship::turnClockwise() { angle += turnSpeed; }
void Ship::turnCounterClockwise() { angle -= turnSpeed; }
// ANCHOR_END: rotate

// ANCHOR: position
MathVector2D<float> Ship::getPosition() { return position; }
// ANCHOR_END: position

// ANCHOR: angle
int Ship::radToDeg(float rad) {
  return (int)(rad * (DEGREES_IN_A_CIRCLE / (2 * PI)));
}
int Ship::getAngleDeg() { return radToDeg(angle); }
// ANCHOR_END: angle
