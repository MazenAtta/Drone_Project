#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>


int main() {

    int ch;
    char command[10];

    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                //strcpy(command, "UP");
                break;
            case KEY_DOWN:
                //strcpy(command, "DOWN");
                break;
            case KEY_LEFT:
                //strcpy(command, "LEFT");
                break;
            case KEY_RIGHT:
                //strcpy(command, "RIGHT");
                break;
            case 's':
                //strcpy(command, "STOP");
                break;
            case 'r':
                //strcpy(command, "RESET");
                break;
            default:
                continue;
        }

        // Send command to the Blackboard Server
        write(STDOUT_FILENO, command, strlen(command) + 1);
    }

    endwin();
    return 0;
}
