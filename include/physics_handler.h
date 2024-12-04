#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H

// Drone mass and friction constants
#define DRONE_MASS 1.0
#define DRONE_FRICTION 0.1

// Struct to represent drone dynamics
typedef struct {
    float x, y;           // Position
    float vx, vy;         // Velocity
    float ax, ay;         // Acceleration
} Drone;

// Initialize the drone's physics
void init_drone(Drone *drone);

// Update drone dynamics based on external forces
void update_drone(Drone *drone, float force_x, float force_y);

// Apply friction to slow down the drone naturally
void apply_friction(Drone *drone);

#endif
