#include "ui_handler.h"

void display_status(float x, float y, const char *status) {
    mvprintw(0, 0, "Drone Position: X=%f.4, Y=%f.4 | Status: %s", x, y, status);
}

void display_legend() {
    int height = 0, width = 0;
    getmaxyx(stdscr, height, width);
    mvprintw(height - 2, width - 38, "Use Arrow keys | 'r' Reset | 'q' Quit");
}

