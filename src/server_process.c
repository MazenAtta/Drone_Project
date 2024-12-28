#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include "obstacle_target_handler.h"
#include "physics_handler.h"

#define BUFFER_SIZE 256

typedef struct {
    int command;
    int Obstacle_x[MAX_OBSTACLES], Obstacle_y[MAX_OBSTACLES];
    int Target_x[MAX_TARGETS], Target_y[MAX_TARGETS], target_id[MAX_TARGETS];
    int game_pause;
    int game_start;
    int game_over;
    int game_reset;
    int score;
    int level;
} Game;

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";
    const char *obstacle_ask = "/tmp/obstacle_ask";
    const char *obstacle_receive = "/tmp/obstacle_receive";
    const char *target_ask = "/tmp/target_ask";
    const char *target_receive = "/tmp/target_receive";
    const char *watchdog_ask = "/tmp/watchdog_ask";

    // Open named pipes
    int fd_output_ask = open(output_ask, O_WRONLY);
    int fd_output_receive = open(output_receive, O_RDONLY);
    int fd_input_ask = open(input_ask, O_WRONLY);
    int fd_input_receive = open(input_receive, O_RDONLY);
    int fd_obstacle_receive = open(obstacle_receive, O_RDONLY);
    int fd_target_receive = open(target_receive, O_RDONLY);
    
    if (fd_output_ask < 0 || fd_output_receive < 0 || fd_input_ask < 0 || fd_input_receive < 0 || fd_obstacle_receive < 0 || fd_target_receive < 0) {
        error_exit("Failed to open named pipes");
    }

    fd_set read_fds;
    Drone drone;
    Obstacle obstacles[MAX_OBSTACLES];
    Target targets[MAX_TARGETS];
    Game game;

    game.game_start = 0;
    // Determine the maximum file descriptor value for select
    int max_fd = fd_input_receive;
    if (fd_output_receive > max_fd) max_fd = fd_output_receive;
    if (fd_obstacle_receive > max_fd) max_fd = fd_obstacle_receive;
    if (fd_target_receive > max_fd) max_fd = fd_target_receive;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(fd_input_receive, &read_fds);
        FD_SET(fd_output_receive, &read_fds);
        FD_SET(fd_obstacle_receive, &read_fds);
        FD_SET(fd_target_receive, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }

        if (FD_ISSET(fd_obstacle_receive, &read_fds) && FD_ISSET(fd_target_receive, &read_fds) && game.game_start == 0) {
            read(fd_obstacle_receive, &obstacles, sizeof(Obstacle));
            read(fd_target_receive, &targets, sizeof(Target));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles[i].x;
                game.Obstacle_y[i] = obstacles[i].y;
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets[i].x;
                game.Target_y[i] = targets[i].y;
                game.target_id[i] = targets[i].id;
            }
            write(fd_output_ask, &game, sizeof(Game));
            printf("Obstacles and targets sent to output window\n");
        }

        if (FD_ISSET(fd_input_receive, &read_fds) && game.game_start == 0) {
            ssize_t bytes_read = read(fd_input_receive, &game, sizeof(Game));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles[i].x;
                game.Obstacle_y[i] = obstacles[i].y;
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets[i].x;
                game.Target_y[i] = targets[i].y;
                game.target_id[i] = targets[i].id;
            }
            if (game.game_start == 1) {
                printf("Game started\n");
            }   
        }

        if (FD_ISSET(fd_input_receive, &read_fds) && game.game_start == 1) {
            ssize_t bytes_read = read(fd_input_receive, &game, sizeof(Game));
            // Update game state
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                game.Obstacle_x[i] = obstacles[i].x;
                game.Obstacle_y[i] = obstacles[i].y;
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                game.Target_x[i] = targets[i].x;
                game.Target_y[i] = targets[i].y;
                game.target_id[i] = targets[i].id;
            }
            if (bytes_read > 0) {
                write(fd_output_ask, &game, sizeof(Game)); // Send command to output window
            } else {
                perror("Error reading from fd_input_receive");
            }
        }

        if (FD_ISSET(fd_output_receive, &read_fds) && game.game_start == 1) {
            ssize_t bytes_read = read(fd_output_receive, &drone, sizeof(Drone));
            if (bytes_read > 0) {
                write(fd_input_ask, &drone, sizeof(Drone));
            } else {
                perror("Error reading from fd_output_receive");
            }
        }
    }

    // Close named pipes
    close(fd_input_receive);
    close(fd_output_receive);
    close(fd_output_ask);
    close(fd_input_ask);
    close(fd_obstacle_receive);
    close(fd_target_receive);

    return 0;
}