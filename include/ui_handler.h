#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <ncurses.h>

// Display the status window with the drone's position and state
void display_status(int x, int y, const char *status);

// Display a key legend at the bottom of the screen
void display_legend();

#endif
