/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Authors: marcio Monteiro and Marleson Graf
 */

#include "Semaphore.h"
#include "Thread.h"

int main() {
	Thread t1, t2, t3, t4, t5;
	// Semaphores s1 and s2 are initialized with 2
	Semaphore s1(2), s2(2);

	// t1 calls p() in s1
	s1.p();
	// s1 value updates to 1

	// Change to thread t2
	Thread::switch_threads(&t2);

	// t2 calls p() in s1
	s1.p();
	// s1 value updates to 0

	// Change to thread t3
	Thread::switch_threads(&t3);

	// t3 calls p() in s1
	s1.p();
	// s1 value updates to -1
	// Thread t3 is blocked


	// Change to thread t1
	Thread::switch_threads(&t1);

	// t1 calls v() in s1
	s1.v();
	// s1 value updates to 0
	// Thread t3 is resumed

	// Change to thread t3
	Thread::switch_threads(&t3);

	// t3 calls p() in s2
	s2.p();
	// s2 value updates to 1

	// Change to thread t4
	Thread::switch_threads(&t4);

	// t4 calls p() in s2
	s2.p();
	// s2 value updates to 0

	// Change to thread t2
	Thread::switch_threads(&t2);

	// t2 calls v() in s1
	s1.v();
	// s1 value updates to 1

	// Change to thread t5
	Thread::switch_threads(&t5);

	// t5 calls p() in s2
	s2.p();
	// s2 value updates to -1
	// Thread t5 is blocked

	// Change to thread t3
	Thread::switch_threads(&t3);

	// t3 calls v() in s2
	s2.v();
	// s2 value updates to 0
	// Thread t5 is resumed

	// t3 calls v() in s1
	s1.v();
	// s1 value updates to 2

	// Change to thread t5
	Thread::switch_threads(&t5);

	// t5 calls v() in s2
	s2.v();
	// s2 value updates to 1

	// Change to thread t4
	Thread::switch_threads(&t4);

	// t4 calls v() in s2
	s2.v();
	// s2 value updates to 2

	return 0;
}
