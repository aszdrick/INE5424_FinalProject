/*
 * Semaphore.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: aszdrick
 */

#include "../HAL/inc/io.h"
#include "Semaphore.h"
#include "sys/alt_stdio.h"
#include "system.h"
#include "Thread.h"

const unsigned long long Semaphore::BASE_ADDRESS = SEMAPHORE_BASE;

Semaphore::Semaphore(unsigned v) {
	alt_printf("Semaphore::Semaphore(%d)\n", v);

	//	DISABLE CPU INTERRUPTIONS
	IOWR_32DIRECT(BASE_ADDRESS, input::COMMAND, command::CREATE);

	IOWR_32DIRECT(BASE_ADDRESS, input::DATA, v);

	int status = IORD_32DIRECT(BASE_ADDRESS, output::STATUS);

	while (!(status & mask::DONE)) {
		status = IORD_32DIRECT(BASE_ADDRESS, output::STATUS);
	}

	if (status & mask::ERROR) {
		throw 666;
	}

	sem_id = IORD_32DIRECT(BASE_ADDRESS, output::DATA);
	//	ENABLE CPU INTERRUPTIONS
}

Semaphore::~Semaphore() {
	alt_putstr("Semaphore::~Semaphore()\n");

	//	LOCK CPU

	IOWR_32DIRECT(BASE_ADDRESS, input::COMMAND, command::DESTROY);

	IOWR_32DIRECT(BASE_ADDRESS, input::SEMAPHORE, sem_id);
	// TODO: remove this shit
	IOWR_32DIRECT(BASE_ADDRESS, input::DATA, 0);

	//	ENABLE CPU INTERRUPTIONS
}

void Semaphore::p() {
	alt_putstr("Semaphore::p()\n");

	unsigned int thread = Thread::RUNNING;

	//	DISABLE CPU INTERRUPTIONS

	IOWR_32DIRECT(BASE_ADDRESS, input::COMMAND, command::DOWN);

	IOWR_32DIRECT(BASE_ADDRESS, input::SEMAPHORE, sem_id);

	IOWR_32DIRECT(BASE_ADDRESS, input::DATA, thread);

	int status = IORD_32DIRECT(BASE_ADDRESS, output::STATUS);

	while (!(status & mask::DONE)) {
		status = IORD_32DIRECT(BASE_ADDRESS, output::STATUS);
	}

	//	ENABLE CPU INTERRUPTIONS

	if (status & mask::ERROR) {
		throw 666;
	}

	if (status & mask::BLOCK) {
		// Block Current Thread
	}
}

void Semaphore::v() {
	alt_putstr("Semaphore::v()\n");

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
