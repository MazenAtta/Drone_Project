CC = gcc
CFLAGS = -Wall -Wextra -Iinclude     # Added -Iinclude to specify the include directory
LDFLAGS = -lncurses -lm              # Link ncurses and math libraries
SRCDIR = src
INCDIR = include
OBJ = $(SRCDIR)/main.o $(SRCDIR)/ncurses_handler.o $(SRCDIR)/ui_handler.o $(SRCDIR)/physics_handler.o $(SRCDIR)/obstacle_target_handler.o

all: drone_simulator drone_process

drone_simulator: $(OBJ)
	$(CC) -o drone_simulator $(OBJ) $(LDFLAGS)

drone_process: $(SRCDIR)/drone_process.o $(SRCDIR)/physics_handler.o
	$(CC) -o drone_process $(SRCDIR)/drone_process.o $(SRCDIR)/physics_handler.o $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o drone_simulator drone_process
