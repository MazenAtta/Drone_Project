#ifndef OBSTACLE_TARGET_HANDLER_H
#define OBSTACLE_TARGET_HANDLER_H

#include <ncurses.h>

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5

typedef struct {
    int x, y;
} Obstacle;

typedef struct {
    int x, y;
    int id;
} Target;
// Initialize obstacles and targets
void init_obstacles(Obstacle obstacles[]);
void init_targets(Target targets[]);

// Randomly generate obstacles and targets
void generate_obstacles(Obstacle obstacles[]);
void generate_targets(Target targets[]);

// Draw obstacles and targets
void draw_obstacles(Obstacle obstacles[]);
void draw_targets(Target targets[]);

#endif
