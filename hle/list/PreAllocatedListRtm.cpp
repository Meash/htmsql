#include "PreAllocatedListRtm.h"
#include <immintrin.h>
#include <stdio.h>

const int MAX_RETRIES = 100000;

PreAllocatedListRtm::PreAllocatedListRtm() :
		PreAllocatedList() {
}
PreAllocatedListRtm::~PreAllocatedListRtm() {
}

void PreAllocatedListRtm::insertTail(ListItem * item) {
	int failures = 0;
	retry: if (_xbegin() == _XBEGIN_STARTED) {
		PreAllocatedList::insertTail(item);
		_xend();
	} else if (++failures < MAX_RETRIES) {
		goto retry;
	} else {
		fprintf(stderr, "Max failures %d reached in function %s\n", failures,
				"insertHead");
		exit(1);
	}
}

ListItem* PreAllocatedListRtm::createListItem(int data,
		ListItem * prev, ListItem * next) {
	int failures = 0;
	retry: if (_xbegin() == _XBEGIN_STARTED) {
		ListItem * result = PreAllocatedList::createListItem(data, prev, next);
		_xend();
		return result;
	} else if (++failures < MAX_RETRIES) {
		goto retry;
	} else {
		fprintf(stderr, "Max failures %d reached in function %s\n", failures,
				"createListItem");
		exit(1);
	}
}

void PreAllocatedListRtm::removeFromList(ListItem * item) {
	int failures = 0;
	retry: if (_xbegin() == _XBEGIN_STARTED) {
		List::removeFromList(item);
		_xend();
	} else if (++failures <= MAX_RETRIES) {
		goto retry;
	} else {
		fprintf(stderr, "Max failures %d reached in function %s\n", failures,
				"remove");
		exit(1);
	}
}

ListItem * PreAllocatedListRtm::get(int data) {
	int failures = 0;
	retry: if (_xbegin() == _XBEGIN_STARTED) {
		ListItem * result = List::get(data);
		_xend();
		return result;
	} else if (++failures < MAX_RETRIES) {
		goto retry;
	} else {
		fprintf(stderr, "Max failures %d reached in function %s\n", failures,
				"contains");
		exit(1);
	}
}
