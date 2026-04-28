CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c
OBJ = $(SRC:.c=.o)
EXEC = ans

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)