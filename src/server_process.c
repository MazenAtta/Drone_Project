#include "blackboard_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    // Define paths for named pipes
    const char *input_ask = "/tmp/input_ask";
    const char *input_receive = "/tmp/input_receive";
    const char *output_ask = "/tmp/output_ask";
    const char *output_receive = "/tmp/output_receive";

    // Open named pipes
    int fd_output_receive = open(output_receive, O_RDONLY);
    int fd_output_ask = open(output_ask, O_WRONLY);
    int fd_input_ask = open(input_ask, O_WRONLY);
    int fd_input_receive = open(input_receive, O_RDONLY);

    fd_set read_fds;
    char buffer[BUFFER_SIZE];
    int max_fd = fd_input_receive > fd_output_receive ? fd_input_receive : fd_output_receive;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(fd_input_receive, &read_fds);
        FD_SET(fd_output_receive, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }

        if (FD_ISSET(fd_input_receive, &read_fds)) {
            ssize_t bytes_read = read(fd_input_receive, buffer, BUFFER_SIZE);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Null-terminate the string
                write(fd_output_ask, buffer, bytes_read + 1); // Send command to output window
            } else {
                perror("Error reading from fd_input_receive");
            }
        }

        if (FD_ISSET(fd_output_receive, &read_fds)) {

            ssize_t bytes_read = read(fd_output_receive, buffer, BUFFER_SIZE);
            printf("bytes_read: %ld\n", bytes_read);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Null-terminate the string
                write(fd_input_ask, buffer, bytes_read + 1); // Send new position to input window
            } else {
                perror("Error reading from fd_output_receive");
            }
        }
    }

    // Close named pipes
    close(fd_input_receive);
    close(fd_output_receive);
    close(fd_output_ask);
    close(fd_input_ask);

    return 0;
}