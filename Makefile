.PHONY: all clean test

CFLAGS += -Wall -Wextra

SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)
NAME = liburlmatch.a

all: $(NAME)

$(NAME): $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJ): $(HDR)

clean:
	$(MAKE) -C test clean
	rm -f $(OBJ) $(NAME)

test: all
	$(MAKE) -C test
