#include "ncurses_handler.h"
#include "ui_handler.h"
#include "obstacle_target_handler.h"




void init_ncurses() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);  // Obstacles
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Targets
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    clear();
    refresh();
}


void draw_drone(Drone *drone, char *status) {
    clear();             // Clear the screen
    mvaddch((float)drone->y, (float)drone->x, DRONE_SYMBOL);
    display_status((float)drone->x,(float)drone->y, status);
    display_legend();
    refresh();           // Refresh the screen
}

int handle_input(Drone *drone, char *status) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:    drone->command_force_y = -COMMAND_FORCE_STEP; strcpy(status, "Moving Up"); break;
        case KEY_DOWN:  drone->command_force_y = COMMAND_FORCE_STEP; strcpy(status, "Moving Down"); break;
        case KEY_LEFT:  drone->command_force_x = -COMMAND_FORCE_STEP; strcpy(status, "Moving Left"); break;
        case KEY_RIGHT: drone->command_force_x = COMMAND_FORCE_STEP; strcpy(status, "Moving Right"); break;
        case 's':       drone->prev_total_force_x = 0; drone->prev_total_force_y = 0; drone->command_force_x = 0; drone->command_force_y = 0; strcpy(status, "Stopped"); break;        
        case 'q':        return QUIT; // Quit
        case 'r':        strcpy(status, "Resetting"); return RESET; // Reset
        default:         drone->command_force_x = 0; drone->command_force_y = 0; strcpy(status, "Moving"); break;
    }
    return APPLY_MOVEMENT;
}

void close_ncurses() {
    endwin();            // Terminate ncurses mode
}
