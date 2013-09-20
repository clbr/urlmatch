SRC = $(wildcard *.c)
TARGETS = $(SRC:.c=)

CFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)
	@echo ./run-tests.sh $(TARGETS)

$(TARGETS): ../liburlmatch.a

clean:
	rm -f *.o $(TARGETS)
