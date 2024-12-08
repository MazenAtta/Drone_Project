#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <unistd.h>

// Drone state structure
typedef struct {
    float x, y;
    float vx, vy;
} DroneState;

// Obstacle state structure
typedef struct {
    int x, y;
} ObstacleState;

// Target state structure
typedef struct {
    int x, y;
    int id;
} TargetState;

// Pipes for communication
typedef struct {
    int drone_pipe[2];        // Pipe for drone process
    int obstacle_pipe[2];     // Pipe for obstacle process
    int target_pipe[2];       // Pipe for target process
} BlackboardPipes;

#endif
