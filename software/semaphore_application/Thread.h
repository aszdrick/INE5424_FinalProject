/*
 * Thread.h
 *
 *  Created on: Oct 27, 2016
 *      Authors: marcio Monteiro and Marleson Graf
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "sys/alt_stdio.h"

class Thread {
 public:
	Thread();
	~Thread();

	static Thread* running();
	static Thread* switch_threads(Thread*);
 private:
	static Thread* RUNNING;
};

inline Thread::Thread() {
	alt_printf("Thread::Thread() : creating thread %x\n\n", this);
	if (!RUNNING) {
		RUNNING = this;
		alt_printf("Thread %x is running.\n\n", this);
	}
}

inline Thread::~Thread() {
	alt_printf("Thread::~Thread() : Thread %x destroyed.\n\n", this);
}

inline Thread* Thread::running() {
	return RUNNING;
};

inline Thread* Thread::switch_threads(Thread* thread) {
	alt_printf("Switching threads...\n", thread);
	Thread* old = RUNNING;
	alt_printf("Previous thread running: %x\n", old);
	RUNNING = thread;
	alt_printf("New thread running: %x\n\n", RUNNING);
	return old;
}

#endif /* THREAD_H_ */
