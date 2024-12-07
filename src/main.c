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

    srand(time(NULL));  // Seed for random number generation

    // Initialize ncurses and entities
    init_ncurses();
    init_drone(&drone);
    init_obstacles(obstacles);
    init_targets(targets);

    generate_obstacles(obstacles);
    generate_targets(targets);
    // Initial draw of drone, obstacles, and targets
    update_drone(&drone);
    draw_drone(&drone, status);
    draw_obstacles(obstacles);
    draw_targets(targets);

    while (1) 
    {
        int action = handle_input(&drone, status);  // Handle user input
        
        if (action == 1) break;  // Quit program
        if (action == 2 || action == 3) init_drone(&drone);  // Reset drone to initial state
        
        // Change obstacle and target positions if action == 3
        if (action == 4) {
            generate_obstacles(obstacles);
            generate_targets(targets);
        }

        // Update drone physics and redraw
        calculate_total_forces(&drone, obstacles, targets);
        update_drone(&drone);
        draw_drone(&drone, status);

        // Always draw obstacles and targets (their positions persist unless updated)
        draw_obstacles(obstacles);
        draw_targets(targets);

        // Delay for smooth updates (uncomment if needed)
        // usleep(50000);  // 50ms delay
    }

    close_ncurses();  // Cleanup ncurses
    return 0;
}

