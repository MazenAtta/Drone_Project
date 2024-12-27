#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

void display_controls() {
    mvprintw(1, 5, "Drone Controller Simulation");
    mvprintw(3, 5, "[↖] [↑] [↗]");
    mvprintw(4, 5, "[←] [■] [→]");
    mvprintw(5, 5, "[↙] [↓] [↘]");
    mvprintw(7, 5, "Other Commands:");
    mvprintw(8, 5, "[s] Stop  |  [r] Reset  |  [q] Quit");
}

void display_drone_state(float x, float y, float vx, float vy) {
    mvprintw(10, 5, "Drone Position: X = %.2f, Y = %.2f", x, y);
    mvprintw(11, 5, "Velocity: VX = %.2f, VY = %.2f", vx, vy);
    refresh();
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
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

    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    display_controls();
    float x = 10.0, y = 10.0, vx = 0.0, vy = 0.0;
    display_drone_state(x, y, vx, vy);

    int ch;
    char command[10];
    char buffer[256];
    while (1) 
    {
        ch = getch();
        switch (ch) {
            case 'w': case 'a': case 's': case 'd': case 'q':
                snprintf(command, sizeof(command), "%c", ch);
                write(fd_receive, command, strlen(command) + 1);
                break;
            default:
                refresh();
        }

        read(fd_ask, buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            sscanf(buffer, "X=%f,Y=%f,VX=%f,VY=%f", &x, &y, &vx, &vy);
            display_drone_state(x, y, vx, vy);}

    }

    endwin();
    close(fd_receive);
    return 0;
}
