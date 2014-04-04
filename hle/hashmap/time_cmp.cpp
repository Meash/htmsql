#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/time.h> // time measurement
#include <ctime> // clock
#include <vector> // time measurement
#include <set>
#include <cmath> // sqrt
#include <limits.h> // INT_MAX
#include "../lock_functions/LockType.h"
#include "HashMap.h"
#include "HashMap-rtm.h"
#include "../../util.h"
#include "../../Stats.h"

#define CORES 4

#define VALUE_RANGE INT_MAX
std::vector<int> values;
std::vector<int> operations;
#define OPERATION_INSERT 1
#define OPERATION_CONTAINS 0
#define OPERATION_REMOVE -1

#define RAND(limit) rand_gerhard(limit)
//#define RAND(limit) rand() % (limit)

// NOTE: not used, approach does not work
void checkResult(HashMap *map) {
	return; // vector is not thread-safe
	std::set<int> expectedValues;
	// collect expectedValues
	std::vector<int>::iterator vIt = values.begin();
	std::vector<int>::const_iterator oIt = operations.begin();
	for (; vIt != values.end(); vIt++, oIt++) {
		int value = *vIt;
		int operation = *oIt;
		switch (operation) {
		case OPERATION_INSERT:
			expectedValues.insert(value);
			break;
		case OPERATION_REMOVE:
			expectedValues.erase(value);
			break;
			// nothing changed for OPERATION_CONTAINS
		}
	}

	for (std::set<int>::iterator sIt = expectedValues.begin();
			sIt != expectedValues.end(); sIt++) {
		int value = *sIt;
		if (!map->remove(value)) {
			printf("ERROR: map is supposed to contain %d\n", value);
		}
	}
	if (map->countElements() > 0) {
		printf("ERROR: Map is not supposed to contain any more values\n");
	}
}

/**
 * Use the values- and operations-vector partitioned for each thread.
 */
void createValues(int threads_count, int repeats, int probability_insert,
		int probability_remove, int probability_contains) {
	for (int r = 0; r < threads_count * repeats; r++) {
		int rnd_op = RAND(
				probability_insert + probability_remove + probability_contains);
		int rnd_val = RAND(VALUE_RANGE);
		values.push_back(rnd_val);
		if (rnd_op < probability_insert) { // insert
			operations.push_back(OPERATION_INSERT);
		} else if (rnd_op < probability_insert + probability_remove) { // remove
			operations.push_back(OPERATION_INSERT);
		} else { // contains
			operations.push_back(OPERATION_CONTAINS);
		}
	}
}

/**
 * Performs random operations, based on the defined probabilities.
 */
void random_operations(int tid, HashMap *map, int repeats, int base_inserts,
		int probability_insert, int probability_remove,
		int probability_contains) {
	// insert base-values to achieve a defined base-size of the map
	for (int i = 0; i < base_inserts; i++) {
		int rnd_val = RAND(VALUE_RANGE);
		map->insert(rnd_val);
	}
	// operations run
	for (int r = 0; r < repeats; r++) {
		int rnd_op = RAND(
				probability_insert + probability_remove + probability_contains);
		int rnd_val = RAND(VALUE_RANGE);
		if (rnd_op < probability_insert) { // insert
			map->insert(rnd_val);
		} else if (rnd_op < probability_insert + probability_remove) { // remove
			map->remove(rnd_val);
		} else { // contains
			map->contains(rnd_val);
		}
	}
}

///
/// main
///
int main(int argc, char *argv[]) {
	// arguments
	int num_threads = CORES;
	int repeats = 100000, loops = 100, probability_insert = 25,
			probability_remove = 25, probability_contains = 50, base_inserts =
					5000, lockType = -1;
	int sizes[] = { 1, 100, 1000, 10000 };
	int *arg_values[] =
			{ &repeats, &num_threads, &loops, &probability_insert,
					&probability_remove, &probability_contains, &base_inserts,
					&lockType };
	const char *identifier[] = { "-r", "-n", "-l", "-pi", "-pr", "-pc", "-bi",
			"-t" };
	handle_args(argc, argv, 8, arg_values, identifier);

	printf("Threads:      %d\n", num_threads);
	printf("Loops:        %d\n", loops);
	printf("Repeats:      %d\n", repeats);
	printf("(base_inserts=%d, prob_ins=%d, prob_rem=%d, prob_con=%d)\n",
			base_inserts, probability_insert, probability_remove,
			probability_contains);
	printf("\n");

	// define locktypes
	LockType::EnumType lockTypesEnum[] = {
	//
			LockType::PTHREAD,
			//
			LockType::ATOMIC_EXCH_SPEC,
			//
			LockType::HLE_EXCH_SPEC,
			//
			LockType::RTM
	//
			};
	int lockTypesCount, lockTypesMin, lockTypesMax;
	lockTypesCount = sizeof(lockTypesEnum) / sizeof(lockTypesEnum[0]);
	if (lockType > -1) {
		lockTypesMin = lockType;
		lockTypesMax = lockType + 1;
	} else {
		lockTypesMin = 0;
		lockTypesMax = lockTypesCount;
	}
	LockType lockTypes[lockTypesCount];
	for (int t = lockTypesMin; t < lockTypesMax; t++) {
		lockTypes[t].init(lockTypesEnum[t]);
	}

	printf("Size;");
	const char *appendings[2];
	appendings[0] = "ExpectedValue";
	appendings[1] = "Stddev";
	LockType::printHeaderRange(lockTypes, lockTypesMin, lockTypesMax,
			appendings, 2);
	for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
		printf("%d", sizes[s]);
		std::cout.flush();

		if (sizes[s] < 100)
			repeats = 1000;

		// run all lock-types
		for (int t = lockTypesMin; t < lockTypesMax; t++) {
			// use a loop to check the time more than once --> normalize
			Stats stats;
			for (int l = 0; l < loops; l++) {
				// init
//				HashMap map(mapSize, lockTypes[t]);
				HashMap * map = NULL;
				switch (lockTypes[t].enum_type) {
				case LockType::EnumType::RTM:
					map = new HashMapRtm(sizes[s]);
					break;
				default:
					map = new HashMap(sizes[s], lockTypes[t]);
					break;
				}

				// measure
				struct timeval start, end;
				gettimeofday(&start, NULL);

				std::thread threads[num_threads];
				for (int i = 0; i < num_threads; i++) {
					threads[i] = std::thread(random_operations, i, map, repeats,
							base_inserts, probability_insert,
							probability_remove, probability_contains);
				}
				// wait for threads
				for (int i = 0; i < num_threads; i++) {
					threads[i].join();
				}

				// measure time
				gettimeofday(&end, NULL);
				float elapsed_millis = (end.tv_sec - start.tv_sec) * 1000
						+ (end.tv_usec - start.tv_usec) / 1000;
				float throughput_per_milli = repeats / elapsed_millis;
				stats.addValue(throughput_per_milli);

				// check result
//				checkResult(&map);

				delete map;
			} // end of loops loop

			printf(";%.2f;%.2f", stats.getExpectedValue(),
					stats.getStandardDeviation());
			std::cout.flush();
		} // end of locktype-loop
		printf("\n");
	} // end of repeats loop
}
