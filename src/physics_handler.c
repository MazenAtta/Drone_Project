#include "physics_handler.h"
#include <math.h>

void init_drone(Drone *drone) {
    drone->x = 10.0000;
    drone->y = 10.0000;
    drone->vx = 0;
    drone->vy = 0;
    drone->ax = 0;
    drone->ay = 0;
    drone->prev_x = 10;
    drone->prev_y = 10;
    drone->command_force_x = 0;
    drone->command_force_y = 0;
    drone->prev_total_force_x = 0;
    drone->prev_total_force_y = 0;
}

void calculate_total_forces(Drone *drone, Obstacle obstacles[], Target targets[]) {
    drone->force_x = drone->command_force_x + drone->prev_total_force_x;
    drone->force_y = drone->command_force_y + drone->prev_total_force_y;

  /*  // Repulsive forces
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x != -1) {
            float dx = drone->x - obstacles[i].x;
            float dy = drone->y - obstacles[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {
                float factor = REPULSIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                *force_x += factor * (dx / distance);
                *force_y += factor * (dy / distance);
            }
        }
    }

    // Attractive forces
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1) {
            float dx = targets[i].x - drone->x;
            float dy = targets[i].y - drone->y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance > 0) {  // Avoid zero division
                *force_x += ATTRACTIVE_CONSTANT * dx / distance;
                *force_y += ATTRACTIVE_CONSTANT * dy / distance;
            }
        }
    }*/
}

void update_drone(Drone *drone) {
    // Calculate velocity before updating position
    drone->vx = (drone->x - drone->prev_x) / TIME_STEP;
    drone->vy = (drone->y - drone->prev_y) / TIME_STEP;

    // Update position using Euler’s method
    float new_x = 2 * drone->x - drone->prev_x + (TIME_STEP * TIME_STEP / DRONE_MASS) * 
                  (drone->force_x - DRONE_FRICTION * drone->vx);

    float new_y = 2 * drone->y - drone->prev_y + (TIME_STEP * TIME_STEP / DRONE_MASS) * 
                  (drone->force_y - DRONE_FRICTION * drone->vy);

    // Update state
    drone->prev_x = drone->x;
    drone->prev_y = drone->y;
    drone->x = new_x;
    drone->y = new_y;
    // Update previous total forces
    drone->prev_total_force_x = drone->force_x;
    drone->prev_total_force_y = drone->force_y;
    
}
