#include <stdlib.h>
#include "ncurses_handler.h"

int main() {
    int x = 10, y = 10;  // Initial drone position
    char status[20] = "Stopped";

    init_ncurses();      // Initialize ncurses
    draw_drone(x, y, status);

    while (1) {
        if (handle_input(&x, &y, status)) break;
        draw_drone(x, y, status);
    }

    close_ncurses();     // Cleanup ncurses
    return 0;
}
