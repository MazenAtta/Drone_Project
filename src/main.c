#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "ncurses_handler.h"
#include "physics_handler.h"
#include "obstacle_target_handler.h"

int main() {
    Drone drone;
    char status[20] = "Stopped";

    Obstacle obstacles[MAX_OBSTACLES];
    Target targets[MAX_TARGETS];

    srand(time(NULL));

    init_ncurses();
    init_drone(&drone);
    update_drone(&drone);
    draw_drone(&drone, status);
    while (1) 
    {

        int action = handle_input(&drone, status);
        if (action == 1) break;  // Quit
        if (action == 2) init_drone(&drone);  // Reset

        calculate_total_forces(&drone, obstacles, targets);
        update_drone(&drone);
        draw_drone(&drone, status);

        //usleep(50000);  // 50ms delay
    }

    close_ncurses();
    return 0;
}
