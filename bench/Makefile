.PHONY: all clean

CFLAGS += -Wall -Wextra
CPPFLAGS += -I ..
LDFLAGS += -lz

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
NAME = bench

all: $(NAME)

$(NAME): $(OBJ) ../liburlmatch.a

clean:
	rm -f $(OBJ) $(NAME)
