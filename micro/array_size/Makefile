CC=g++
LDFLAGS=-fgnu-tm -mrtm -lpthread

# Test
test: test.cpp ../util.cpp
	g++ -o test -fgnu-tm -mrtm -O2 test.cpp ../util.cpp -lpthread

test.s: test.cpp ../util.cpp
	gcc -S -masm=intel -fgnu-tm -mrtm -O2 test.cpp ../util.cpp -lpthread

# Suite
SOURCES_SUITE = suite.cpp ../../util.cpp ../../Stats.cpp
suite: $(SOURCES_SUITE)
	$(CC) -o $@ $(LDFLAGS) -O0 $(SOURCES_SUITE)

# simple size
SOURCES_SIZE_SIMPLE=size_simple.cpp ../../util.cpp
size_simple: $(SOURCES_SIZE_SIMPLE)
	$(CC) -o $@ $(LDFLAGS) -O0 $(SOURCES_SIZE_SIMPLE)

# read_write_mix
SOURCES_READ_WRITE_MIX=read-write-mix.cpp ../../util.cpp ../../Stats.cpp
read_write_mix: $(SOURCES_READ_WRITE_MIX)
	$(CC) -o $@ $(LDFLAGS) -O0 $(SOURCES_READ_WRITE_MIX)