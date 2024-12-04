#ifndef NCURSES_HANDLER_H
#define NCURSES_HANDLER_H

#include <ncurses.h>
#include <string.h>
#include <physics_handler.h>

#define DRONE_SYMBOL '+'
#define APPLY_MOVEMENT 0
#define QUIT 1
#define RESET 2
#define FORCE_RIGHT 1
#define FORCE_UP -1.0
#define FORCE_DOWN 1.0
#define FORCE_LEFT -1.0

// Initialize ncurses
void init_ncurses();

// Draw the drone at a specific position
void draw_drone(Drone *drone, char *status);

// Handle key input and update drone position
int handle_input(float *force_x, float *force_y, char *status);

// Close ncurses properly
void close_ncurses();

#endif

