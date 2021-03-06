/*
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <cmath> // sqrt
#include <immintrin.h> // rtm
#include "../../util.h"
#include "../../Stats.h"

int main(int argc, char *argv[]) {
	int loops = 10, inner_loops = 100, rmin = 10000, rmax =
			20000000;
	float rstep = 1.15;
	int *values[] = { &loops, &inner_loops, &rmin, &rmax/*, &rstep*/ };
	const char *identifier[] = { "-l", "-il", "-rmin", "-rmax"/*, "-rstep"*/ };
	handle_args(argc, argv, /*5*/4, values, identifier);

	printf("Loops:    %d\n", loops);
	printf("Clocks:  %d - %d with steps of %.2f\n", rmin, rmax, rstep);
	printf("\n");

	FILE *out = stdout;
//	std::string filename = "clocktime_nop-micro.csv";
//	FILE * out = fopen(filename.c_str(), "w");

	//	printf("Clocks;Attempts;Failures;Failure rate [%%]\n");
	fprintf(out, "Clocks;ExpectedValue;Stddev\n");

	for (int clocks = rmin; clocks <= rmax; clocks *= rstep) {
		fprintf(out, "%d", clocks);
		Stats stats;
		for (int l = 0; l < loops; l++) {
			int failures = 0;
			for (int p = 0; p < inner_loops; p++) {
				int code = _xbegin();
				if (code == _XBEGIN_STARTED) {
					for (int i = 0; i < clocks; i++) {
						asm volatile("nop");
					}
					_xend();
				} else {
//					printf("Code: %d\n", code); // 0 for interrupts
					failures++;
				}
			}
			float failure_rate = (float) failures / inner_loops * 100;
			stats.addValue(failure_rate);
		}


//		float failure_rate = (float) failures / attempts * 100;
//		fprintf(out, "%d;%d;%.2f", attempts, failures, failure_rate);

//		float expected_value = (float) failures / loops;
//		int successful = loops - failures;
//		float variance = (float) (successful * failures) / (loops * loops);

		printf(";%.2f;%.2f", stats.getExpectedValue(), stats.getStandardDeviation());
		fprintf(out, "\n");
	}

	return 0;
}
