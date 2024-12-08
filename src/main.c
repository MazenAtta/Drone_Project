#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include "ncurses_handler.h"
#include "blackboard.h"

int main() {
    BlackboardPipes pipes;

    // Create pipes for communication
    if (pipe(pipes.drone_pipe) == -1 ||
        pipe(pipes.obstacle_pipe) == -1 ||
        pipe(pipes.target_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork drone process
    if (fork() == 0) {
        close(pipes.drone_pipe[0]);
        dup2(pipes.drone_pipe[1], STDOUT_FILENO);
        execl("./drone_process", "drone_process", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Fork obstacle process
    if (fork() == 0) {
        close(pipes.obstacle_pipe[0]);
        dup2(pipes.obstacle_pipe[1], STDOUT_FILENO);
        execl("./obstacle_process", "obstacle_process", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Fork target process
    if (fork() == 0) {
        close(pipes.target_pipe[0]);
        dup2(pipes.target_pipe[1], STDOUT_FILENO);
        execl("./target_process", "target_process", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Close write ends in the parent process
    close(pipes.drone_pipe[1]);
    close(pipes.obstacle_pipe[1]);
    close(pipes.target_pipe[1]);

    fd_set read_fds;
    char buffer[256];

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(pipes.drone_pipe[0], &read_fds);
        FD_SET(pipes.obstacle_pipe[0], &read_fds);
        FD_SET(pipes.target_pipe[0], &read_fds);

        int max_fd = pipes.target_pipe[0] + 1;

        if (select(max_fd, &read_fds, NULL, NULL, NULL) > 0) {
            if (FD_ISSET(pipes.drone_pipe[0], &read_fds)) {
                read(pipes.drone_pipe[0], buffer, sizeof(buffer));
                printf("Drone Update: %s\n", buffer);
            }

            if (FD_ISSET(pipes.obstacle_pipe[0], &read_fds)) {
                read(pipes.obstacle_pipe[0], buffer, sizeof(buffer));
                printf("Obstacle Update: %s\n", buffer);
            }

            if (FD_ISSET(pipes.target_pipe[0], &read_fds)) {
                read(pipes.target_pipe[0], buffer, sizeof(buffer));
                printf("Target Update: %s\n", buffer);
            }
        }
    }

    return 0;
}
