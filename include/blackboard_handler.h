#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 256

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

// Pipe file descriptors
int keyboard_pipe[2];     // For receiving user input from the keyboard process
int input_process_pipe[2];    // Pipe for input window process
int output_process_pipe[2];      // Pipe for output window process


// argument lists
char *keyboard_process_arg_list[] = {"./keyboard_process", NULL };
char *input_process_arg_list[] = { "konsole","-e","/input_window_process",NULL };
char *output_process_arg_list[] = {"konsole","-e","/output_window_process", NULL };





// Error handling and process spawning functions
void error_exit(const char *msg);

// Process spawning function
int spawn(const char *program, char **arg_list);



#endif