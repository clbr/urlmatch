SRC = $(wildcard *.c)
TARGETS = $(SRC:.c=)

CFLAGS += -Wall -Wextra
CPPFLAGS += -I ..
LDFLAGS += -lz

.PHONY: all clean

all: $(TARGETS)
	@./run-tests.sh $(TARGETS)

$(TARGETS): ../liburlmatch.a

clean:
	rm -f *.o $(TARGETS)
