#include "ncurses_handler.h"
#include "ui_handler.h"

void init_ncurses() {
    initscr();           // Initialize ncurses
    cbreak();            // Disable line buffering
    noecho();            // Disable character echo
    keypad(stdscr, TRUE);// Enable special keys (arrows)
    curs_set(0);         // Hide the cursor
    clear();             // Clear the screen
    refresh();
}

void draw_drone(int x, int y, char *status) {
    clear();             // Clear the screen
    mvaddch(y, x, DRONE_SYMBOL);  // Place drone at (x, y)
    display_status(x, y, status);
    display_legend();
    refresh();           // Refresh the screen
}

int handle_input(int *x, int *y, char *status) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:     (*y)--; strcpy(status, "Moving"); break;
        case KEY_DOWN:   (*y)++; strcpy(status, "Moving"); break;
        case KEY_LEFT:   (*x)--; strcpy(status, "Moving"); break;
        case KEY_RIGHT:  (*x)++; strcpy(status, "Moving"); break;
        case 'q':        return 1;  // Quit
        case 'r':        *x = 10; *y = 10; strcpy(status, "Resetting"); break;
        default:         strcpy(status, "Stopped"); break;
    }
    return 0;
}

void close_ncurses() {
    endwin();            // Terminate ncurses mode
}
