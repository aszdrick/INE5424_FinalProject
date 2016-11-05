/*
 * Thread.h
 *
 *  Created on: Oct 27, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "sys/alt_stdio.h"

class Thread {
	static unsigned thread_counter;
	static Thread* RUNNING;
 public:
	explicit Thread(unsigned = thread_counter);
	~Thread();

	unsigned identifier();
	static Thread* running();
	static Thread* switch_threads(Thread*);
 private:
	unsigned id;
};

inline Thread::Thread(unsigned id) : id(id) {
	// alt_printf("Thread::Thread() : creating thread %x\n\n", id);
	if (!RUNNING) {
		RUNNING = this;
		// alt_printf("Thread %x is running.\n\n", id);
	}
}

inline Thread::~Thread() {
	// alt_printf("Thread::~Thread() : Thread %x destroyed.\n\n", id);
}

inline unsigned Thread::identifier() {
	return id;
}

inline Thread* Thread::running() {
	return RUNNING;
};

inline Thread* Thread::switch_threads(Thread* thread) {
	// alt_printf("Switching threads...\n", thread);
	Thread* old = RUNNING;
	// alt_printf("Previous thread running: %x\n", old->identifier());
	RUNNING = thread;
	// alt_printf("New thread running: %x\n\n", RUNNING->identifier());
	return old;
}

#endif /* THREAD_H_ */
