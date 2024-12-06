CC = gcc
CFLAGS = -Wall -Wextra -lncurses
LDFLAGS = -lm  # Link math library
SRCDIR = src
INCDIR = include
OBJ = $(SRCDIR)/main.o $(SRCDIR)/ncurses_handler.o $(SRCDIR)/ui_handler.o $(SRCDIR)/physics_handler.o $(SRCDIR)/obstacle_target_handler.o

all: drone_simulator

drone_simulator: $(OBJ)
	$(CC) -o drone_simulator $(OBJ) $(CFLAGS) $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -I$(INCDIR) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(SRCDIR)/*.o drone_simulator
