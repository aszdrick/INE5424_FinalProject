// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#ifndef THREAD_H_
#define THREAD_H_

#include "sys/alt_stdio.h"

// Simplified representation of a thread, with some
// functionalities to emulate an scheduler.
class Thread {
    // Identifies the current number of threads in system.
    static unsigned thread_counter;
    // Identifies the current running thread.
    static Thread* RUNNING;
 public:
    // Thread constructor.
    // Receives a comprehensive id for thread.
    // If nothing is passed, uses thread_counter as id.
    // Also, if no thread is running, set created thread to running.
    explicit Thread(unsigned = thread_counter);
    
    // Thread destructor. Could be implicitly defined, this definition
    // has been created solely to add verbosity. 
    ~Thread();

    // Returns the identifier of thread.
    unsigned identifier();
    
    // Returns the running thread.
    static Thread* running();
    
    // Switches RUNNING with the thread passed as argument.
    static Thread* switch_threads(Thread*);
 private:
    // Thread identifier.
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
