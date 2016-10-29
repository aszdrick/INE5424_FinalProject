/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Authors: marcio Monteiro and Marleson Graf
 */

#include "Semaphore.h"
#include "Thread.h"

int main() {
	Thread t1;
	Thread t2;

	Semaphore s1;
	Semaphore s2;
	Semaphore s3;
	Semaphore s4;
	Semaphore s5;
	Semaphore s6;
	Semaphore s7;
	Semaphore s8;

	try {
		Semaphore s9;
	} catch (...) {

	}


	s1.p();

	Thread::switch_threads(&t2);
	s1.p();

	Thread::switch_threads(&t1);
	s1.v();

	Thread::switch_threads(&t2);
	s1.v();

	Thread::switch_threads(&t1);
	s1.v();
	s1.p();

	Thread::switch_threads(&t2);
	s1.p();

	return 0;
}
