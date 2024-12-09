#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void init_ncurses() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}

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

int main() {
    init_ncurses();
    display_controls();

    float x = 10.0, y = 10.0, vx = 0.0, vy = 0.0;
    char buffer[256];

    while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0) {
        sscanf(buffer, "%f %f %f %f", &x, &y, &vx, &vy);
        display_drone_state(x, y, vx, vy);
    }

    endwin();
    return 0;
}
