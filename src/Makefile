CFLAGS = -Wall -std=c++11 -Wextra -Wno-unused -Wno-unused-variable -O1

OBJS = auxiliares.cc savings.cc greedy.cc sweep.cc kmeans.cc annealing.cc neighborhood.cc
TESTS = test/test_utils.cc test/test_greedy.cc test/test_sweep.cc

.PHONY: all build test clean dirs

all: dirs build test

build: dirs main.cc lib/tiempo.h point.h $(OBJS)
	g++ $(CFLAGS) -o build/main main.cc $(OBJS) -I.

test: dirs point.h test/test_runner.cc $(TESTS)
	g++ $(CFLAGS) -o build/tests test/test_runner.cc $(OBJS) $(TESTS) -I.

clean:
	rm -f build/* out/*

dirs:
	mkdir -p build out resources
