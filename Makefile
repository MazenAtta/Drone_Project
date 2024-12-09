# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude     # Include directory for headers
LDFLAGS = -lncurses -lm              # Link ncurses and math libraries

# Directories
SRCDIR = src
INCDIR = include

# Source Files
BLACKBOARD_SRC = $(SRCDIR)/blackboard_handler.c $(SRCDIR)/blackboard_process.c
DRONE_SRC = $(SRCDIR)/drone_process.c $(SRCDIR)/physics_handler.c
OBSTACLE_SRC = $(SRCDIR)/obstacle_process.c $(SRCDIR)/obstacle_target_handler.c
TARGET_SRC = $(SRCDIR)/target_process.c $(SRCDIR)/obstacle_target_handler.c
INPUT_WINDOW_SRC = $(SRCDIR)/input_window_process.c $(SRCDIR)/ncurses_handler.c
OUTPUT_WINDOW_SRC = $(SRCDIR)/output_window_process.c $(SRCDIR)/ncurses_handler.c
KEYBOARD_SRC = $(SRCDIR)/keyboard_process.c $(SRCDIR)/ncurses_handler.c

# Object Files
BLACKBOARD_OBJ = $(BLACKBOARD_SRC:.c=.o)
DRONE_OBJ = $(DRONE_SRC:.c=.o)
OBSTACLE_OBJ = $(OBSTACLE_SRC:.c=.o)
TARGET_OBJ = $(TARGET_SRC:.c=.o)
INPUT_WINDOW_OBJ = $(INPUT_WINDOW_SRC:.c=.o)
OUTPUT_WINDOW_OBJ = $(OUTPUT_WINDOW_SRC:.c=.o)
KEYBOARD_OBJ = $(KEYBOARD_SRC:.c=.o)

# All Targets
all: blackboard_process drone_process obstacle_process target_process input_window_process output_window_process keyboard_process

# Main Blackboard Process
blackboard_process: $(BLACKBOARD_OBJ)
	$(CC) -o blackboard_process $(BLACKBOARD_OBJ) $(LDFLAGS)

# Drone Process
drone_process: $(DRONE_OBJ)
	$(CC) -o drone_process $(DRONE_OBJ) $(LDFLAGS)

# Obstacle Process
obstacle_process: $(OBSTACLE_OBJ)
	$(CC) -o obstacle_process $(OBSTACLE_OBJ) $(LDFLAGS)

# Target Process
target_process: $(TARGET_OBJ)
	$(CC) -o target_process $(TARGET_OBJ) $(LDFLAGS)

# Input Window Process
input_window_process: $(INPUT_WINDOW_OBJ)
	$(CC) -o input_window_process $(INPUT_WINDOW_OBJ) $(LDFLAGS)

# Output Window Process
output_window_process: $(OUTPUT_WINDOW_OBJ)
	$(CC) -o output_window_process $(OUTPUT_WINDOW_OBJ) $(LDFLAGS)

# Keyboard Process
keyboard_process: $(KEYBOARD_OBJ)
	$(CC) -o keyboard_process $(KEYBOARD_OBJ) $(LDFLAGS)

# Compile .c files into .o files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(SRCDIR)/*.o blackboard_process drone_process obstacle_process target_process input_window_process output_window_process keyboard_process
