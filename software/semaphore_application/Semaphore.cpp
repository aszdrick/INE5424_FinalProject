/*
 * Semaphore.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: aszdrick
 */

#include "Semaphore.h"
#include "system.h"
#include "/opt/altera/13.0sp1/nios2eds/components/altera_nios2/HAL/inc/io.h"


const unsigned long long Semaphore::BASE = SEMAPHORE_BASE;

Semaphore::Semaphore(unsigned v = 1) {
	//	sem_cmd = (unsigned int∗) 0x41300000;
	//	sem_thr = (unsigned int∗) 0x41300004;

	//	CPU::int_disable();

	//	∗sem_cmd = (0x20000000 | v);
	//	sem_id = (∗sem_thr & 0x000000FF);

	//	CPU::int_enable();

}

Semaphore::~Semaphore() {
	//	CPU::int_disable();
	//
	//	∗sem_cmd = (0x40000000 | (sem_id << 16));
	//
	//	CPU::int_enable();
}

void Semaphore::p() {
	// TSC::Time Stamp tmp = mytsc.time stamp();
//	Thread∗ thr = Thread::self();
//
//	CPU::int_disable();
//
//	∗sem_thr = (unsigned int) thr;
//	∗sem_cmd = (0x60000000 | (sem_id << 16));
//	auto status = ∗sem_cmd;
//
//	CPU::int_enable();
//
//	// kout << (mytsc.time stamp() − tmp) << ”\n”;
//
//	if (status & STAT_ERROR) {
//		Machine::panic();
//	}
//
//	if (status & STAT_BLOCK) {
//		// Block Current Thread
//		thr−>suspend();
//	}
}

void Semaphore::v() {
	// TSC::Time Stamp tmp = mytsc.time stamp();
//	Thread∗ thr;
//
//	CPU::int_disable();
//
//	∗sem_cmd = (0x80000000 | (sem_id << 16));
//	auto status = ∗sem_cmd;
//	thr = (Thread∗)∗sem_thr;
//
//	CPU::int_enable();
//
//	// kout << (mytsc.time stamp() − tmp) << ”\n”;
//
//	if (status & STAT_ERROR) {
//		Machine::panic();
//	}
//
//	if (status & STAT_RESUME) {
//		thr −>resume();
//	}
}
