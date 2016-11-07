// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#ifndef CPU_H_
#define CPU_H_

#include "HAL/inc/sys/alt_irq.h"

// Simple CPU abstraction to NIOS II.
class CPU {
 public:
    // Disable interruptions of NIOS II.
    static void int_disable();
    // Enable interruptions of NIOS II.
    static void int_enable();
 private:
    // Stores interruption context.
    static alt_irq_context context;
};

inline void CPU::int_disable() {
    context = alt_irq_disable_all();
}

inline void CPU::int_enable() {
    alt_irq_enable_all(context);
}

#endif /* CPU_H_ */
