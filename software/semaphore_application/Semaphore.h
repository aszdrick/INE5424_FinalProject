/*
 * Semaphore.h
 *
 *  Created on: Oct 25, 2016
 *      Author: aszdrick
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

class Semaphore {
	static const unsigned long long BASE;
 public:
    enum STATUS {
        STAT_BLOCK  = 0x10000000,
        STAT_RESUME = 0x08000000,
        STAT_FULL   = 0x04000000,
        STAT_ERROR  = 0x02000000,
        STAT_DONE   = 0x01000000,
    };

    Semaphore(unsigned);
    ~Semaphore();

	void p();
	void v();

 private:
	unsigned int sem_id;
	volatile unsigned int* sem_cmd;
	volatile unsigned int* sem_thr;
};

#endif /* SEMAPHORE_H_ */
