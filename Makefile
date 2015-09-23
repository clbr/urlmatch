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
	mkdir -p -m 755 $(DESTDIR)$(PREFIX)/include
	install -m 644 urlmatch.h $(DESTDIR)$(PREFIX)/include
	mkdir -p -m 755 $(DESTDIR)$(PREFIX)/lib
	install -m 644 $(NAME) $(DESTDIR)$(PREFIX)/lib
