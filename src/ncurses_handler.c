#include "ncurses_handler.h"

void init_ncurses() {
    initscr();           // Initialize ncurses
    cbreak();            // Disable line buffering
    noecho();            // Disable character echo
    keypad(stdscr, TRUE);// Enable special keys (arrows)
    curs_set(0);         // Hide the cursor
    clear();             // Clear the screen
    refresh();
}

void draw_drone(int x, int y) {
    clear();             // Clear the screen
    mvaddch(y, x, DRONE_SYMBOL);  // Place drone at (x, y)
    mvprintw(0, 0, "Use arrow keys to move. Press 'q' to quit, 'r' to reset."); // Instructions
    refresh();           // Refresh the screen
}

int handle_input(int *x, int *y) {
    int ch = getch();    // Get user input
    switch (ch) {
        case KEY_UP:     (*y)--; break;
        case KEY_DOWN:   (*y)++; break;
        case KEY_LEFT:   (*x)--; break;
        case KEY_RIGHT:  (*x)++; break;
        case 'q':        return 1;  // Quit
        case 'r':        *x = 10; *y = 10; break; // Reset position
        default:         break;
    }
    return 0;            // Continue running
}

void close_ncurses() {
    endwin();            // Terminate ncurses mode
}
