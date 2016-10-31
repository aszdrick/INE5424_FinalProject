/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Authors: marcio Monteiro and Marleson Graf
 */

#include "Semaphore.h"
#include "Thread.h"

int main() {
	Thread t1, t2;
	Semaphore s1;

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
