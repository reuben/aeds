SOURCES := tp1.c graph.c pqueue.c queue.c

CFLAGS    := -x c -O2 -Wno-unused-result
LDFLAGS   := -lm

ifdef BENCHMARK
	CFLAGS += -DBENCHMARK
endif

all: tp1

tp1: $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) tp1
