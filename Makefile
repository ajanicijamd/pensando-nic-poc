CFLAGS += -g
LDFLAGS += -lstdc++

SOURCES := nicctl_runner.cpp NicMeasurePensando.cpp
HEADERS := NicMeasurePensando.h NicMeasure.h

all: build
build: nicctl-runner prog
nicctl-runner: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

prog: main.cpp
	$(CC) $(CFLAGS) -o $@ main.cpp $(LDFLAGS)

clean:
	rm -rf nicctl-runner prog

rebuild: clean build

run: build
	./prog 43000070-0100-0000-4242-0490812c77b0
