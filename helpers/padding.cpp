#include <stdio.h>
#include <stdlib.h>

#define PADDING 1

#include "../hle/lock_functions/LockType.h"

#include "../hle/shared_counter/PaddedCounter.h"
#include "../hle/shared_counter/ThreadsafeCounter.h"
#include "../hle/shared_counter/RtmCounter.h"

#include "../hle/bank/entities/Account.h"
#include "../hle/bank/entities/ThreadsafeAccount.h"

#include "../hle/list/ListItem.h"

#include "../hle/hashmap/HashMap.h"

int main(int argc, char * argv[]) {
	printf("L1 DCache size:         %d KByte\n", 32);
	printf("L1 Cache line size:     %d Byte (multiples: %d, %d)\n", 64, 2 * 64,
			3 * 64);
	printf("\n");

	printf("unsigned size:          %lu Byte\n", sizeof(unsigned));
	printf("int size:               %lu Byte\n", sizeof(int));
	int intarr[16];
	printf("int[16] size:           %lu Byte\n", sizeof(intarr));
	printf("\n");

	printf("LockType Size:          %lu Byte\n", sizeof(LockType));
	printf("\n");

	// shared counter
	printf("PaddedCounter Size:     %lu Byte\n", sizeof(PaddedCounter));
	printf("ThreadsafeCounter Size: %lu Byte\n", sizeof(ThreadsafeCounter));
	printf("RtmCounter Size:        %lu Byte\n", sizeof(RtmCounter));
	printf("\n");
	// bank
	printf("Account Size:           %lu Byte\n", sizeof(Account));
	printf("ThreadsafeAccount Size: %lu Byte\n", sizeof(ThreadsafeAccount));
	printf("\n");
	// list
	printf("ListItem Size:          %lu Byte\n", sizeof(ListItem));
	printf("\n");
	// hashmap
	printf("HashMap Size:           %lu Byte\n", sizeof(HashMap));
	printf("LockedBucket Size:      %lu Byte\n", sizeof(LockedBucket));
	printf("LinkedListItem Size:    %lu Byte\n", sizeof(LinkedListItem));
}
