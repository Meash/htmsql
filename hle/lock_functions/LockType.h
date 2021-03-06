#ifndef LOCKTYPE_H_
#define LOCKTYPE_H_

#include "def.h"
//#include <boost/thread.hpp>

class LockType {
public:
	enum EnumType {
		NONE, PTHREAD,
//		PTHREAD_HLE,
		ATOMIC_EXCH_BUSY,
		ATOMIC_EXCH_SPEC,
		ATOMIC_EXCH_HLE_BUSY,
		ATOMIC_EXCH_HLE_SPEC,
		ATOMIC_TAS_BUSY,
		ATOMIC_TAS_SPEC,
		ATOMIC_TAS_HLE_BUSY,
		ATOMIC_TAS_HLE_SPEC,
		RTM,
		RTM_SMART,
		HLE_TAS_BUSY,
		HLE_TAS_SPEC,
		HLE_EXCH_BUSY,
		HLE_EXCH_SPEC,
		HLE_EXCH_SPEC_CHECKED,
		HLE_ASM_EXCH_BUSY,
		HLE_ASM_EXCH_SPEC,
		HLE_ASM_EXCH_ASM_SPEC
	};
	EnumType enum_type;
//	static const LockType PTHREAD;
//	static const LockType CPP11MUTEX;
//	static const LockType ATOMIC_EXCH;
//	static const LockType ATOMIC_EXCH_HLE;
//	static const LockType ATOMIC_EXCH_HLE2;
//	static const LockType ATOMIC_TAS;
//	static const LockType ATOMIC_TAS_HLE;
//	static const LockType RTM;
//	static const LockType HLE_TAS;
//	static const LockType HLE_EXCH;

	LockType();
	LockType(LockType::EnumType enum_type);
	void init(LockType::EnumType enum_type);

	/**
	 * Actual lock-functions. These functions will refer to one of the mutex-lock-functions.
	 * To call the method of this function-pointer, use the following syntax:
	 * <code>
	 * 		// Given LockType *locker
	 * 		(locker->*(locker->lock))();
	 * 		(locker->*(locker->unlock))();
	 *
	 * 		// Given LockType locker
	 * 		(locker.*(locker.lock))();
	 * 		(locker.*(locker.unlock))();
	 * </code>
	 * The methodology of this is as follows: Get the function-pointer that the object holds.
	 * Then call the function of the object that the pointer is pointing to.
	 */
	void (LockType::*lock)();
	void (LockType::*unlock)();

	// printing
	static const char* getEnumText(LockType::EnumType e);
	static void printHeader(LockType *lockTypes[], int size,
			FILE *out = stdout);
	static void printHeader(LockType lockTypes[], int size, FILE *out = stdout);
	static void printHeader(LockType *lockTypes[], int size, const char *appendings[],
			int appendings_length, FILE *out = stdout);
	static void printHeader(LockType lockTypes[], int size, const char *appendings[],
			int appendings_length, FILE *out = stdout);
	static void printHeaderRange(LockType lockTypes[], int from, int to,
			FILE *out = stdout);
	static void printHeaderRange(LockType lockTypes[], int from, int to,
			const char *appendings[], int appendings_length, FILE *out = stdout);
private:
	void (*type_lock_function)(type *lock);
	void (*type_unlock_function)(type *lock);
	// mutexes
	pthread_mutex_t p_mutex = PTHREAD_MUTEX_INITIALIZER;
//	pthread_mutex_t p_mutex_hle = PTHREAD_MUTEX_HLE_ADAPTIVE_NP;
	type type_mutex = 0;
//	short padding[1024];
	// different lock functions for different mutexes
	void no_lock();
	void no_unlock();
	void pthread_lock();
	void pthread_unlock();
//	void pthread_hle_lock();
//	void pthread_hle_unlock();
	void type_lock();
	void type_unlock();
	void rtm_call_lock();
	void rtm_call_unlock();
};

#endif // LOCKTYPE_H_
