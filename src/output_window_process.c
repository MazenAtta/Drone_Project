#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;
    float total_force_x;
    float total_force_y;
} DroneState;

void init_ncurses() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    clear();
    refresh();
}

void draw_drone(float x, float y) {
    clear();
    mvprintw((int)y, (int)x, "+");  // Draw the drone as a '+'
    refresh();
}

void update_drone_state(DroneState *drone, char command) {
    switch (command) {
        case 'w': drone->y -= 1; break;
        case 's': drone->y += 1; break;
        case 'a': drone->x -= 1; break;
        case 'd': drone->x += 1; break;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <ask_fifo> <receive_fifo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ask_fifo = argv[1];
    const char *receive_fifo = argv[2];

    int fd_receive = open(receive_fifo, O_WRONLY);
    int fd_ask = open(ask_fifo, O_RDONLY | O_NONBLOCK);

    init_ncurses();

    DroneState drone = {10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    char buffer[256];
    draw_drone(drone.x, drone.y);
    while (1) {
        ssize_t bytes_read = read(fd_ask, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            update_drone_state(&drone, buffer[0]);
            draw_drone(drone.x, drone.y);
            snprintf(buffer, sizeof(buffer), "X=%.2f,Y=%.2f", drone.x, drone.y);
            write(fd_receive, buffer, strlen(buffer) + 1);
        }
    }

    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}