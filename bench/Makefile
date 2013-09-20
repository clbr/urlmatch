.PHONY: all clean

CFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
NAME = bench

all: $(NAME)

$(NAME): $(OBJ)

clean:
	$(MAKE) -C test clean
	rm -f $(OBJ) $(NAME)
