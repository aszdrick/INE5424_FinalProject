/*
 * Semaphore.cpp
 *
 *  Created on: Oct 25, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#include "HAL/inc/io.h"

#include "CPU.h"
#include "Semaphore.h"
#include "sys/alt_stdio.h"
#include "system.h"
#include "Thread.h"

const unsigned long long Semaphore::BASE_ADDRESS = SEMAPHORE_BASE;

Semaphore::Semaphore(unsigned v) {
	// alt_printf("Semaphore::Semaphore(%d) : ", v);

	CPU::int_disable();

	IOWR_32DIRECT(BASE_ADDRESS, input::COMMAND, command::CREATE);
	IOWR_32DIRECT(BASE_ADDRESS, input::DATA, v);

	int status = read_status();

	if (status & mask::ERROR) {
		// alt_printf("\n\nERROR: maximum of semaphores reached!\n\n");
		throw -1;
	}

	id = IORD_32DIRECT(BASE_ADDRESS, output::DATA);

	CPU::int_enable();

	// alt_printf("Semaphore %x created.\n\n", id);
}

Semaphore::~Semaphore() {
	// alt_putstr("Semaphore::~Semaphore() : ");

	CPU::int_disable();

	execute_command(command::DESTROY);

	int status = read_status();

	if (status & mask::ERROR) {
		// alt_printf("\n\nERROR: destroying semaphore with threads blocked!\n\n");
		throw -2;
	}

	CPU::int_enable();

	 // alt_printf("Semaphore %x destroyed.\n\n", id);
}

void Semaphore::p() {
	// alt_printf("Calling p() in Semaphore %d...\n", id);

	Thread* thread = Thread::running();

	CPU::int_disable();

	execute_command(command::DOWN, reinterpret_cast<int>(thread));

	int status = read_status();

	if (status & mask::ERROR) {
		if (status & mask::BLOCK) {
			// alt_printf("\n\nERROR: No more space available to store blocked thread!\n\n");
			throw -3;
		} else {
			// alt_printf("ERROR: Attempt to call p() in non-existent semaphore!\n\n");
			throw -4;
		}
	}

	CPU::int_enable();

	if (status & mask::BLOCK) {
		// alt_printf("\nBlocking thread %x...\n\n", thread->identifier());
		Thread::switch_threads(0);
	}
}

void Semaphore::v() {
	// alt_printf("Calling v() in Semaphore %d...\n", id);

	CPU::int_disable();

	execute_command(command::UP);

	int status = read_status();

	if (status & mask::ERROR) {
		// alt_printf("ERROR: Attempt to call v() in non-existent semaphore!\n\n");
		throw -4;
	}

	CPU::int_enable();

	if (status & mask::RESUME) {
		Thread* thread = reinterpret_cast<Thread*>(read_data());
		// alt_printf("\nResuming thread %x...\n", thread->identifier());
	}

	// alt_printf("\n");
}

void Semaphore::execute_command(unsigned command, unsigned data) {
	IOWR_32DIRECT(BASE_ADDRESS, input::COMMAND, command);

	IOWR_32DIRECT(BASE_ADDRESS, input::SEMAPHORE, id);

	IOWR_32DIRECT(BASE_ADDRESS, input::DATA, data);
}

unsigned Semaphore::read_data() {
	return IORD_32DIRECT(Semaphore::BASE_ADDRESS, output::DATA);
}

unsigned Semaphore::read_status() {
	unsigned status = IORD_32DIRECT(Semaphore::BASE_ADDRESS, output::STATUS);
	while (!(status & (mask::DONE | mask::ERROR))) {
		status = IORD_32DIRECT(Semaphore::BASE_ADDRESS, output::STATUS);
	}
	return status;
}
