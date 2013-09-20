.PHONY: all clean

CFLAGS += -Wall -Wextra

SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)
NAME = liburlmatch.a

all: $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJ): $(HDR)

clean:
	$(MAKE) -C test clean
