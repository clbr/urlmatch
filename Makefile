.PHONY: all clean test

CFLAGS += -Wall -Wextra

PREFIX ?= /usr

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

install: all
	install -m644 -D urlmatch.h $(DESTDIR)$(PREFIX)/include/urlmatch.h
	install -m644 -D $(NAME) $(DESTDIR)$(PREFIX)/lib/$(NAME)
