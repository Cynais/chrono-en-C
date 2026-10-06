CC = gcc

TARGET = perf_example

PERF_MONITORING ?= 0

CFLAGS = -Wall -Wextra
CFLAGS += -O2
CFLAGS += -Icommon/perf
CFLAGS += -Isrc

SRCS = \
	src/main.c \
	src/processing.c


ifeq ($(PERF_MONITORING),1)

CFLAGS += -DPERF_MONITORING=1
SRCS += common/perf/perf_monitor.c

else

CFLAGS += -DPERF_MONITORING=0

endif


OBJS = $(SRCS:.c=.o)


.PHONY: all clean


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f src/*.o
	rm -f common/perf/*.o
	rm -f $(TARGET)