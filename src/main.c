#include <stdlib.h>
#include "ncurses_handler.h"
#include "physics_handler.h"


int main() {
    Drone drone;
    int action = 0;  // Initial drone position
    char status[20] = "Stopped";
    float force_x = 0, force_y = 0;

    init_ncurses();      // Initialize ncurses
    init_drone(&drone); // Initialize drone
    draw_drone(&drone, status); // Draw drone

    while (1) 
    {
        action = handle_input(&force_x, &force_y, status);
        if (action == 1) break;       // Quit
        if (action == 2) init_drone(&drone); // Reset

        update_drone(&drone, force_x, force_y);
        apply_friction(&drone);
        draw_drone(&drone, status);
        force_x = 0; // Reset forces after applying them
        force_y = 0;
    }

    close_ncurses();     // Cleanup ncurses
    return 0;
}
