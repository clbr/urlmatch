.PHONY: all clean

CFLAGS += -Wall -Wextra

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
NAME = liburlmatch.a

all: $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	$(MAKE) -C test clean
