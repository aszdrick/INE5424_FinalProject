/*
 * Semaphore.h
 *
 *  Created on: Oct 25, 2016
 *      Author: aszdrick
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

namespace input {
	enum Input {
		COMMAND = 0,
		SEMAPHORE = 4,
		DATA    = 8
	};
}

namespace output {
	enum Output {
		STATUS = 0,
		DATA = 4
	};
}

namespace command {
	enum Command {
		CREATE = 0x1,
		DESTROY = 0x2,
		DOWN = 0X3,
		UP = 0x4
	};
}

namespace mask {
	enum Mask {
		DONE = 0x1,
		ERROR = 0x2,
		FULL = 0x4,
		BLOCK = 0x8,
		RESUME = 0x16,
	};
}

class Semaphore {
	static const unsigned long long BASE_ADDRESS;
 public:
    Semaphore(unsigned = 1);
    ~Semaphore();

	void p();
	void v();

 private:
	unsigned int sem_id;
	volatile unsigned int* sem_cmd;
	volatile unsigned int* sem_thr;
};

#endif /* SEMAPHORE_H_ */
