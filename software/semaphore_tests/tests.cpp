// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#include "Semaphore.h"
#include "sys/alt_stdio.h"
#include "Thread.h"
#include "tests.h"

void tests::creation_destruction() {
    bool passed = true;

    try {
        Semaphore* tester = new Semaphore();
        delete tester;
    } catch(...) {
        passed = false;
    }

    if (passed) {
        alt_putstr("[PASSED] SEMAPHORE'S CONSTRUCTION & DESTRUCTION\n");
    } else {
        alt_putstr("[FAILED] SEMAPHORE'S CONSTRUCTION & DESTRUCTION\n");
    }    
}

void tests::max_semaphores() {
    bool passed = false;
    Semaphore s1, s2, s3, s4, s5, s6, s7, s8;

    try {
        Semaphore s9;
    } catch (int error) {
        passed = (error == -1);
    }

    if (passed) {
        alt_putstr("[PASSED] TEST MAX SEMAPHORES\n");
    } else {
        alt_putstr("[FAILED] TEST MAX SEMAPHORES\n");
    }
}

void tests::p_decrement() {
    // Implicit switch of running
    Thread thread;
    
    Semaphore tester(1);
    
    tester.p();
    tester.p();
    
    if (Thread::running() == 0) {
        alt_putstr("[PASSED] TEST P DECREMENT\n");
    } else {
        alt_putstr("[FAILED] TEST P DECREMENT\n");
    }

    tester.v();
}

void tests::v_increment() {
    // Implicit switch of running
    Thread thread;

    Semaphore tester(0);

    tester.v();
    tester.p();

    if (Thread::running() != 0) {
        alt_putstr("[PASSED] TEST V INCREMENT\n");
    } else {
        alt_putstr("[FAILED] TEST V INCREMENT\n");
    }
}

void tests::destroy_non_empty_semaphore() {
	Semaphore* tester = new Semaphore(0);
	bool passed = false;

	tester->p();

	try {
		delete tester;
	} catch (int error) {
		passed = (error == -2);
	}

	tester->v();

	delete tester;

    if (passed) {
        alt_putstr("[PASSED] DESTROY NON-EMPTY SEMAPHORE\n");
    } else {
        alt_putstr("[FAILED] DESTROY NON-EMPTY SEMAPHORE\n");
    }
}

void tests::size_init() {
    bool passed = true;
    bool truly_passed = false;
    Thread t1, t2, t3, t4, t5, t6, t7;
    Semaphore tester(7);

    try {
        tester.p();
        
        Thread::switch_threads(&t2);
        tester.p();
        
        Thread::switch_threads(&t3);
        tester.p();
        
        Thread::switch_threads(&t4);
        tester.p();
        
        Thread::switch_threads(&t5);
        tester.p();
        
        Thread::switch_threads(&t6);
        tester.p();
        
        Thread::switch_threads(&t7);
        tester.p();
    } catch (...) {
        passed = false;
    }
    
    tester.p();
    
    truly_passed = (Thread::running() == 0);

    if (passed && truly_passed) {
        alt_putstr("[PASSED] TEST SIZE INITIALIZATION\n");
    } else {
        alt_putstr("[FAILED] TEST SIZE INITIALIZATION\n");
    }

    tester.v();
}

void tests::overflow_size_init() {
    bool passed = true;
    Semaphore tester(1023);
    // Implicit switch of running
    Thread thread;
        
    tester.p();

    if (Thread::running() == 0) {
        alt_putstr("[PASSED] TEST OVERFLOW SIZE INITIALIZATION\n");
    } else {
        alt_putstr("[FAILED] TEST OVERFLOW SIZE INITIALIZATION\n");
    }

    tester.v();
}

void tests::negative_init() {
    Semaphore tester(-1);
    // Implicit switch of running
    Thread thread;

    tester.v();
    tester.v();

    tester.p();

    if (Thread::running() == &thread) {
        alt_putstr("[PASSED] TEST NEGATIVE SIZE INITIALIZATION\n");
    } else {
        alt_putstr("[FAILED] TEST NEGATIVE SIZE INITIALIZATION\n");
    }
}
