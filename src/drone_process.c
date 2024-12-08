#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "physics_handler.h"
#include "blackboard.h"

#define COMMAND_PIPE "/tmp/command_pipe"
#define DRONE_PIPE "/tmp/drone_pipe"

int main() {
    Drone drone;
    init_drone(&drone);

    // Create and open pipes for reading commands and sending updates
    mkfifo(COMMAND_PIPE, 0666);
    mkfifo(DRONE_PIPE, 0666);

    int command_fd = open(COMMAND_PIPE, O_RDONLY);
    int drone_fd = open(DRONE_PIPE, O_WRONLY);

    if (command_fd == -1 || drone_fd == -1) {
        perror("Error opening pipes");
        exit(EXIT_FAILURE);
    }

    char command_buffer[256];
    while (1) {
        // Read command force from the input process
        if (read(command_fd, command_buffer, sizeof(command_buffer)) > 0) {
            sscanf(command_buffer, "%f %f", &drone.command_force_x, &drone.command_force_y);

            // Calculate forces and update drone state
            calculate_total_forces(&drone, NULL, NULL);  // No obstacles/targets in this step
            update_drone(&drone);

            // Send updated drone state to the blackboard server
            char drone_state[256];
            snprintf(drone_state, sizeof(drone_state), "%f %f %f %f",
                     drone.x, drone.y, drone.vx, drone.vy);
            write(drone_fd, drone_state, strlen(drone_state) + 1);
        }
    }

    // Cleanup
    close(command_fd);
    close(drone_fd);
    unlink(COMMAND_PIPE);
    unlink(DRONE_PIPE);

    return 0;
}
