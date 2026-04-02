CC := gcc
CFLAGS := -Wall -Wextra -I../../common

SRCS := unique_snow_flakes.c test_unique_snow_flakes.c
OBJS := $(SRCS:.c=.o)
TARGET := test_unique_snow_flakes

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
 