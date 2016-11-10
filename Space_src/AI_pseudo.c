// Space Invaders AI Pseudo-code

typedef enum SpotType_t { SAFE, UNSAFE, SHOOT_ALIEN, SHOOT_SAUCER };
SpotType_t tankSpots[SCREEN_WIDTH];

// ==================== Safe spots
// Assume all spots are safe...
uint8_t i;
for (i = 0; i < SCREEN_WIDTH; i++) {
  tankSpots = SAFE;
}

// Find unsafe spots
for (alienBullets) {
  // Check if the bullet is currently on screen
  if(isOnScreen) {
    // Get x position

    // When the tank can no longer remain in this position or cannot safely move past this position, it is not safe
    //      ** We are assuming that the tank will always evade when possible...
    // -- Calculate how long it will take bullet to hit x
    uint32_t bulletHitTime = bulletHeight / bulletSpeed;

    // Calculate how long it will take tank to move out of the way
    uint32_t tankEvadeTime; // TODO: We need to find a way to measure time in large numbers to avoid round-off error.
    // Measure these from middle of tank... (?)
    // TODO: take into account other bullets (other unsafe spots)
    if(tank is to the left) {
      tankEvadeTime = (safe space to right of bullet_x - tank_x) / tankSpeed;
    }
    else if (tank is to right) {
      tankEvadeTime = (tank_x - safe spaceLeftOfBullet + tankWidth) / tankSpeed;
    }
    else { // Tank is directly under the bullet
      tankEvadeTime = (tankWidth/2) / tankSpeed;
    }

    if (bulletHitTime < tankEvadeTime + SAFETY_BUFFER) {
      tankSpot[bullet_x] = UNSAFE;
    }

  }
}

// Move to nearest safe spot
