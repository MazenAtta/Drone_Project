#ifndef NCURSES_HANDLER_H
#define NCURSES_HANDLER_H

#include <ncurses.h>
#include <string.h>

#define DRONE_SYMBOL '+'

// Initialize ncurses
void init_ncurses();

// Draw the drone at a specific position
void draw_drone(int x, int y, char *status);

// Handle key input and update drone position
int handle_input(int *x, int *y, char *status);

// Close ncurses properly
void close_ncurses();

#endif

