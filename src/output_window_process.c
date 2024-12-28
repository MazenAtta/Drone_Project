#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "physics_handler.h"
#include "obstacle_target_handler.h"

#define DRONE_SYMBOL '+'
#define FORCE_UP -1
#define FORCE_DOWN 1
#define FORCE_LEFT -1
#define FORCE_RIGHT 1


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

void init_ncurses() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);  // Obstacles
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Targets
    noecho();
    cbreak();
    curs_set(0);
    clear();
    refresh();
}


void draw_drone(Drone *drone) {
    mvaddch((int)drone->y, (int)drone->x, DRONE_SYMBOL | COLOR_PAIR(1));
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

void draw_targets(Target targets[]) {
    attron(COLOR_PAIR(3)); // Green color for targets
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1 && targets[i].y != -1) {
            mvaddch(targets[i].y, targets[i].x, '1' + i);
        }
    }
    attroff(COLOR_PAIR(3));
}



// Initialize the drone's parameters
void init_drone(Drone *drone) {
    drone->x = 10.0;
    drone->y = 10.0;
    drone->vx = 0.0;
    drone->vy = 0.0;
    drone->ax = 0.0;
    drone->ay = 0.0;
    drone->prev_x = 10.0;
    drone->prev_y = 10.0;
    drone->command_force_x = 0.0;
    drone->command_force_y = 0.0;
    drone->prev_total_command_force_x = 0.0;
    drone->prev_total_command_force_y = 0.0;
}

void update_drone_state(Drone *drone, char command) {
    // Reset command forces
    drone->command_force_x = 0.0;
    drone->command_force_y = 0.0;
    switch (command) {
        case 'q': drone->command_force_y = cos(45) * FORCE_UP; drone->command_force_x =  cos(45) * FORCE_LEFT; break;
        case 'w': drone->command_force_y = FORCE_UP; break;
        case 'e': drone->command_force_y = cos(45) * FORCE_UP; drone->command_force_x = cos(45) * FORCE_RIGHT; break;
        case 'a': drone->command_force_x = FORCE_LEFT; break;
        case 's': drone->command_force_y = 0; drone->command_force_x = 0; break;
        case 'd': drone->command_force_x = FORCE_RIGHT; break;
        case 'z': drone->command_force_y = cos(45) * FORCE_DOWN; drone->command_force_x = cos(45) * FORCE_LEFT; break;
        case 'x': drone->command_force_y = FORCE_DOWN; break;
        case 'c': drone->command_force_y = cos(45) * FORCE_DOWN; drone->command_force_x = cos(45) * FORCE_RIGHT; break;
        default: break;
    }
}

// Calculate total forces (command, repulsion, attraction, and drag)
void calculate_total_forces(Drone *drone, Obstacle obstacles[], Target targets[]) {
    // Start with command forces and accumulate continuous command force
    float total_force_x = drone->command_force_x;
    float total_force_y = drone->command_force_y;

    // Calculate drag forces (viscous friction)
    float drag_force_x = -DRONE_FRICTION * drone->vx;
    float drag_force_y = -DRONE_FRICTION * drone->vy;

    // Add drag forces
    total_force_x += drag_force_x;
    total_force_y += drag_force_y;

    // Add repulsive forces
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x != -1) {
            float dx = drone->x - obstacles[i].x;
            float dy = drone->y - obstacles[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {
                float repulsion_factor = REPULSIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += repulsion_factor * (dx / distance);
                total_force_y += repulsion_factor * (dy / distance);
            }
        }
    }

    // (Optional)
    /*// Add attractive forces
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].x != -1) {
            float dx = targets[i].x - drone->x;
            float dy = targets[i].y - drone->y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < PERCEPTION_RADIUS && distance > 0) {  // Avoid zero division
                float attraction_factor = ATTRACTIVE_CONSTANT * (1.0 / distance - 1.0 / PERCEPTION_RADIUS) / (distance * distance);
                total_force_x += attraction_factor * dx / distance;
                total_force_y += attraction_factor * dy / distance;
            }
        }
    }*/
    
    /*if (total_force_x < 1) {
        total_force_x = 0;
    } else if (total_force_y < 1) {
        total_force_y = 0;
    }*/

    // Store the total force for use in the update function
    drone->force_x = total_force_x;
    drone->force_y = total_force_y;
}

// Update the drone's dynamics using Newton's laws
void update_drone(Drone *drone) {
    // Calculate acceleration from the total force
    drone->ax = drone->force_x / DRONE_MASS;
    drone->ay = drone->force_y / DRONE_MASS;

    // Update velocity using v = v + a * TIME_STEP
    drone->vx = drone->vx + drone->ax * TIME_STEP;
    drone->vy = drone->vy + drone->ay * TIME_STEP;

    // Update position using x = x + v * TIME_STEP + 0.5 * a * TIME_STEP^2
    float new_x = drone->x + drone->vx * TIME_STEP + 0.5 * drone->ax * TIME_STEP * TIME_STEP;
    float new_y = drone->y + drone->vy * TIME_STEP + 0.5 * drone->ay * TIME_STEP * TIME_STEP;

    // Ensure new_x is within ±0.5 of the old x
    if (new_x < drone->x - 0.5 || new_x > drone->x + 0.5) {
        new_x = drone->x;
    }

    // Ensure new_y is within ±0.5 of the old y
    if (new_y < drone->y - 0.5 || new_y > drone->y + 0.5) {
        new_y = drone->y;
    }

    // Update the drone's state
    drone->prev_x = drone->x;
    drone->prev_y = drone->y;
    drone->x = new_x;
    drone->y = new_y;

    // Accumulate the command force for continuous effect
    drone->prev_total_command_force_x += drone->command_force_x;
    drone->prev_total_command_force_y += drone->command_force_y;

}




int main() {
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";

    int fd_receive = open(output_receive, O_WRONLY | O_NONBLOCK);
    int fd_ask = open(output_ask, O_RDONLY | O_NONBLOCK);

    init_ncurses();

    Drone drone;
    Obstacle obstacles[MAX_OBSTACLES];
    Target targets[MAX_TARGETS];
    Game game;

    init_drone(&drone);
    clear();
    draw_drone(&drone);
    refresh();

    while (1) 
    {
        
        if (game.game_start == 0) {
            read(fd_ask, &game, sizeof(Game));
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles[i].x = game.Obstacle_x[i];
                obstacles[i].y = game.Obstacle_y[i];
            }
            for (int i = 0; i < MAX_TARGETS; i++) {
                targets[i].x = game.Target_x[i];
                targets[i].y = game.Target_y[i];
                targets[i].id = game.target_id[i];
            }
            clear();
            draw_obstacles(obstacles);
            draw_targets(targets);
            draw_drone(&drone);
            refresh();

        } 
        if (game.game_start == 1) {
            ssize_t bytes_read = read(fd_ask, &game, sizeof(Game));
            if (bytes_read > 0) {
                update_drone_state(&drone, game.command);
                calculate_total_forces(&drone, obstacles, targets);
                update_drone(&drone);
                clear();
                draw_drone(&drone);
                draw_obstacles(obstacles);
                draw_targets(targets);
                refresh();
                write(fd_receive, &drone, sizeof(Drone));
            }
        }
    }

    endwin();
    close(fd_receive);
    close(fd_ask);
    return 0;
}