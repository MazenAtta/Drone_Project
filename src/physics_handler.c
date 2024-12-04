#include "physics_handler.h"

void init_drone(Drone *drone) {
    drone->x = 10;
    drone->y = 10;
    drone->vx = 0;
    drone->vy = 0;
    drone->ax = 0;
    drone->ay = 0;
}

void update_drone(Drone *drone, float force_x, float force_y) {
    // Calculate acceleration from force and mass
    drone->ax = force_x / DRONE_MASS;
    drone->ay = force_y / DRONE_MASS;

    // Update velocity using acceleration
    drone->vx += drone->ax;
    drone->vy += drone->ay;

    // Update position using velocity
    drone->x += drone->vx;
    drone->y += drone->vy;

    // Ensure position stays within screen bounds
    if (drone->x < 0) drone->x = 0;
    if (drone->y < 0) drone->y = 0;
}

void apply_friction(Drone *drone) {
    drone->vx *= (1 - DRONE_FRICTION);
    drone->vy *= (1 - DRONE_FRICTION);
}
