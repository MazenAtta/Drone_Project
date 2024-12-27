#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void create_named_pipe(const char *path) {
    if (mkfifo(path, 0666) == -1) {
        perror("mkfifo");
    }
}

int main() {
    // Define paths for named pipes

    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";

    // Create named pipes

    create_named_pipe(input_ask);
    create_named_pipe(input_receive);
    create_named_pipe(output_ask);
    create_named_pipe(output_receive);

    // Open named pipes in the blackboard process

    int fd_input_ask = open(input_ask, O_RDWR);
    int fd_input_receive = open(input_receive, O_RDWR);
    int fd_output_ask = open(output_ask, O_RDWR);
    int fd_output_receive = open(output_receive, O_RDWR);

    // Spawn the server process
    char *server_process_arg_list[] = {"konsole", "-e","./server_process", NULL};
    if (fork() == 0) {
        execvp(server_process_arg_list[0], server_process_arg_list);
        error_exit("Failed to spawn server process");
    }


    // Spawn the input process in a new terminal window
    char *input_process_arg_list[] = {"konsole", "-e", "./input_window_process", input_ask, input_receive, NULL};
    if (fork() == 0) {
        execvp(input_process_arg_list[0], input_process_arg_list);
        error_exit("Failed to spawn input window process");
    }

    // Spawn the output process in a new terminal window
    char *output_process_arg_list[] = {"konsole", "-e", "./output_window_process", output_ask, output_receive, NULL};
    if (fork() == 0) {
        execvp(output_process_arg_list[0], output_process_arg_list);
        error_exit("Failed to spawn output window process");
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    // Close and remove named pipes

    close(fd_input_ask);
    close(fd_input_receive);
    close(fd_output_ask);
    close(fd_output_receive);


    unlink(input_ask);
    unlink(input_receive);
    unlink(output_ask);
    unlink(output_receive);

    return 0;
}