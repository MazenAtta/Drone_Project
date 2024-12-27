# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lncurses -lm

# Directories
SRCDIR = src
INCDIR = include

# Source files
BLACKBOARD_SRC = $(SRCDIR)/blackboard_process.c
SERVER_SRC = $(SRCDIR)/server_process.c
INPUT_SRC = $(SRCDIR)/input_window_process.c
OUTPUT_SRC = $(SRCDIR)/output_window_process.c

# Executables
BLACKBOARD_EXEC = blackboard_process
SERVER_EXEC = server_process
INPUT_EXEC = input_window_process
OUTPUT_EXEC = output_window_process



# All target
all: $(BLACKBOARD_EXEC) $(SERVER_EXEC) $(INPUT_EXEC) $(OUTPUT_EXEC)

# Build individual executables
$(BLACKBOARD_EXEC): $(BLACKBOARD_SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SERVER_EXEC): $(SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(INPUT_EXEC): $(INPUT_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(OUTPUT_EXEC): $(OUTPUT_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Clean target
clean:
	rm -f $(BLACKBOARD_EXEC) $(SERVER_EXEC) $(INPUT_EXEC) $(OUTPUT_EXEC) $(SRCDIR)/*.o
