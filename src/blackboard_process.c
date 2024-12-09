#include <blackboard_handler.h>

int main() {
    if (pipe(keyboard_pipe) == -1 || pipe(input_process_pipe) == -1 || pipe(output_process_pipe) == -1) {
        error_exit("Pipe creation failed");
    }

    // Argument lists for processes
    char *keyboard_args[] = {"./keyboard_process", NULL};
    char *input_window_args[] = {"./input_window_process", NULL};
    char *output_window_args[] = {"./output_window_process", NULL};

    // Spawn the processes
    if (spawn(keyboard_args[0], keyboard_args) == -1) error_exit("Failed to spawn keyboard process");
    if (spawn(input_window_args[0], input_window_args) == -1) error_exit("Failed to spawn input window process");
    if (spawn(output_window_args[0], output_window_args) == -1) error_exit("Failed to spawn output window process");

    DroneState drone = {10.0, 10.0, 0.0, 0.0, 0.0, 0.0};
    char buffer[BUFFER_SIZE];

    printf("Blackboard Server started...\n");

    fd_set read_fds;
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(keyboard_pipe[0], &read_fds);

        int max_fd = keyboard_pipe[0];
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) > 0) {
            if (FD_ISSET(keyboard_pipe[0], &read_fds)) {
                if (read(keyboard_pipe[0], buffer, sizeof(buffer)) > 0) {
                    printf("Received command: %s\n", buffer);

                    // Update drone state based on command
                    if (strcmp(buffer, "UP") == 0) {
                        drone.y -= 1.0;
                    } else if (strcmp(buffer, "DOWN") == 0) {
                        drone.y += 1.0;
                    } else if (strcmp(buffer, "LEFT") == 0) {
                        drone.x -= 1.0;
                    } else if (strcmp(buffer, "RIGHT") == 0) {
                        drone.x += 1.0;
                    } else if (strcmp(buffer, "STOP") == 0) {
                        drone.vx = 0.0;
                        drone.vy = 0.0;
                    } else if (strcmp(buffer, "RESET") == 0) {
                        drone.x = 10.0;
                        drone.y = 10.0;
                        drone.vx = 0.0;
                        drone.vy = 0.0;
                    }

                    // Send updated state to input and output windows
                    snprintf(buffer, sizeof(buffer), "%f %f %f %f", drone.x, drone.y, drone.vx, drone.vy);
                    write(input_process_pipe[1], buffer, strlen(buffer) + 1);
                    write(output_process_pipe[1], buffer, strlen(buffer) + 1);
                }
            }
        }
    }

    return 0;
}
