.PHONY: all clean

CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = a.out
SRCS   = main.c
CIALLO = ciallo.c
OBJS   = $(SRCS:.c=.o) $(CIALLO:.c=.o)

all: $(TARGET)
	./$<

$(TARGET): $(OBJS)
	$(CC) $^ -o $@
	chmod +x $@

%.o: %.c
ifeq ($(filter $<,$(CIALLO)),)
	$(CC) $(CFLAGS) -O0 -c $< -o $@
else
	$(CC) $(CFLAGS) -O2 -c $< -o $@
endif

clean:
	@rm -f $(TARGET) $(OBJS)
