#include <stdlib.h>
#include "ncurses_handler.h"

int main() {
    int x = 10, y = 10;  // Initial drone position
    init_ncurses();      // Initialize ncurses

    draw_drone(x, y);
    while (1) {
        if (handle_input(&x, &y)) break;
        draw_drone(x, y);
    }

    close_ncurses();     // Cleanup ncurses
    return 0;
}
