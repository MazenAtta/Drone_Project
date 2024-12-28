#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define COLS 50
#define LINES 20

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5


typedef struct {
    int x, y;
    int id;
} Target;

void init_targets(Target targets[]) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets[i].x = -1;
        targets[i].y = -1;
        targets[i].id = i + 1;
    }
}

void generate_targets(Target targets[]) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        // 100% chance of generating targets
        targets[i].x = rand() % COLS;
        targets[i].y = rand() % LINES;
    }
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

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *target_receive = "/tmp/target_receive";

    int fd_receive = open(target_receive, O_WRONLY);

    Target targets[MAX_TARGETS];
    init_targets(targets);

    while (1) {
        generate_targets(targets);
        write(fd_receive, targets, sizeof(targets));
        printf("Targets generated\n");
        break;

    }

    close(fd_receive);
    return 0;
}