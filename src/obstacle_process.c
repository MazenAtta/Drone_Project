#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>

#define COLS 50
#define LINES 20
#define MAX_OBSTACLES 10
#define MAX_TARGETS 5

typedef struct {
    int x, y;
} Obstacle;



void init_obstacles(Obstacle obstacles[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = -1; // Off-screen by default
        obstacles[i].y = -1;
    }
}

void generate_obstacles(Obstacle obstacles[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        // 100% chance of generating obstacles
        obstacles[i].x = rand() % COLS;
        obstacles[i].y = rand() % LINES;
    }
}

void draw_obstacles(Obstacle obstacles[]) {
    attron(COLOR_PAIR(2)); // Orange color for obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x != -1 && obstacles[i].y != -1) {
            mvaddch(obstacles[i].y, obstacles[i].x, 'O');
        }
    }
    attroff(COLOR_PAIR(2));
}


void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *obstacle_receive = "/tmp/obstacle_receive";

    int fd_receive = open(obstacle_receive, O_WRONLY);

    Obstacle obstacles[MAX_OBSTACLES];
    init_obstacles(obstacles);

    while (1) {
        generate_obstacles(obstacles);
        write(fd_receive, obstacles, sizeof(obstacles));
        printf("Obstacles generated\n");
        break;
    }

    close(fd_receive);
    return 0;
}