/*
 * CPU.h
 *
 *  Created on: Oct 27, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#ifndef CPU_H_
#define CPU_H_

#include "HAL/inc/sys/alt_irq.h"

class CPU {
 public:
	static void int_disable();
	static void int_enable();
 private:
	static alt_irq_context context;
};

inline void CPU::int_disable() {
	context = alt_irq_disable_all();
}

inline void CPU::int_enable() {
	alt_irq_enable_all(context);
}

#endif /* CPU_H_ */
