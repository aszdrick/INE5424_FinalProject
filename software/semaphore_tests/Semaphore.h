/*
 * Semaphore.h
 *
 *  Created on: Oct 25, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/// Input enumeration
// Enumeration to specify the target input
namespace input {
	enum Input {
		COMMAND = 0,
		SEMAPHORE = 4,
		DATA = 8
	};
}

/// Output enumeration
// Enumeration to specify the desired output
namespace output {
	enum Output {
		STATUS = 0,
		DATA = 4
	};
}

/// Command enumeration
// Enumeration used pass a command to semaphore
namespace command {
	enum Command {
		CREATE = 0x1,
		DESTROY = 0x2,
		DOWN = 0X3,
		UP = 0x4
	};
}

/// Mask enumeration
// Enumeration used to take a specific status bit from semaphore
namespace mask {
	enum Mask {
		DONE = 0x1,
		ERROR = 0x2,
		FULL = 0x4,
		BLOCK = 0x8,
		RESUME = 0x16,
	};
}

/// Semaphore class
// Simple class to provide interaction with semaphores in hardware
class Semaphore {
	// Base address of Avalon MM interface for semaphore's hardware
	static const unsigned long long BASE_ADDRESS;
 public:
    Semaphore(unsigned = 1);
    ~Semaphore();

    // Lock command (a.k.a. down)
	void p();
	// Unlock command (a.k.a. up)
	void v();

 private:
	// Identifier taken from semaphore's hardware
	unsigned int id;

	void execute_command(unsigned command, unsigned data = 0);
	unsigned read_data();
	unsigned read_status();
};

#endif /* SEMAPHORE_H_ */
