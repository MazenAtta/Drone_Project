#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

int main() {
    init_ncurses();

    float x = 10.0, y = 10.0;
    char buffer[256];

    while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0) {
        sscanf(buffer, "%f %f", &x, &y);
        draw_drone(x, y);
    }

    endwin();
    return 0;
}
