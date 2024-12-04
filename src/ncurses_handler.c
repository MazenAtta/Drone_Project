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

void draw_drone(Drone *drone, char *status) {
    clear();             // Clear the screen
    mvaddch((int)drone->y, (int)drone->x, DRONE_SYMBOL);
    display_status((int)drone->x,(int)drone->y, status);
    display_legend();
    refresh();           // Refresh the screen
}

int handle_input(float *force_x, float *force_y, char *status) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:     *force_y = FORCE_UP; strcpy(status, "Moving Up"); break;
        case KEY_DOWN:   *force_y = FORCE_DOWN; strcpy(status, "Moving Down"); break;
        case KEY_LEFT:   *force_x = FORCE_LEFT; strcpy(status, "Moving Left"); break;
        case KEY_RIGHT:  *force_x = FORCE_RIGHT; strcpy(status, "Moving Right"); break;
        case 'q':        return 1; // Quit
        case 'r':        strcpy(status, "Resetting"); return 2; // Reset
        default:         strcpy(status, "Stopped"); break;
    }
    return 0;
}

void close_ncurses() {
    endwin();            // Terminate ncurses mode
}
