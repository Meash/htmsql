CC=g++
LDFLAGS=-fgnu-tm -mrtm -lpthread

# Sequential
array_size_sequential: array_size_sequential.cpp
	$(CC) -o array_size_sequential -fgnu-tm -mrtm -O2 array_size_sequential.cpp -lpthread

array_size_sequential.s: array_size_sequential.cpp
	gcc -S -masm=intel -fgnu-tm -mrtm -O2 array_size_sequential.cpp -lpthread

# Suite MWE (init_clear + run_seq_read)
SOURCES_SUITE_MWE = suite-mwe.cpp
suite-mwe: $(SOURCES_SUITE_MWE)
	$(CC) -o $@ $(LDFLAGS) -O0 $(SOURCES_SUITE_MWE)

# Suite test
SOURCES_SUITE_TEST = suite_test.cpp ../../../util.cpp
suite_test: $(SOURCES_SUITE_TEST)
	$(CC) -o $@ $(LDFLAGS) -O0 $(SOURCES_SUITE_TEST)