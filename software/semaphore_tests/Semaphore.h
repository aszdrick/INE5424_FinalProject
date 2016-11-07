// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

// Enumerates the target input.
namespace input {
    enum Input {
        COMMAND = 0,
        SEMAPHORE = 4,
        DATA = 8
    };
}

// Enumerates the desired output.
namespace output {
    enum Output {
        STATUS = 0,
        DATA = 4
    };
}

// Enumerates the possible commands to pass to semaphore.
namespace command {
    enum Command {
        CREATE = 0x1,
        DESTROY = 0x2,
        DOWN = 0X3,
        UP = 0x4
    };
}

// Enumerates the masks used to take a specific status bit from semaphore.
namespace mask {
    enum Mask {
        DONE = 0x1,
        ERROR = 0x2,
        FULL = 0x4,
        BLOCK = 0x8,
        RESUME = 0x16,
    };
}

// Simple class to provide interaction with semaphores in hardware.
class Semaphore {
    // Base address of Avalon MM interface for semaphore's hardware.
    static const unsigned long long BASE_ADDRESS;
 public:
    // Semaphore constructor.
    // Executes the create command on semaphore's hardware.
    // If the maximum number of semaphores has been reached, throws -1.
    Semaphore(unsigned = 1);
    
    // Semaphore destructor.
    // Executes the destroy command on semaphore's hardware.
    // If the specified semaphore does not exist in hardware, throws -2.
    ~Semaphore();

    // Lock command. Executes the down command on semaphore's hardware.
    // As result, decrements the semaphore's counter (stored in hardware).
    // If result is negative, the executing thread is blocked and  hardware
    // status returns mask::BLOCK and mask::DONE. For simulation and test
    // puposes, when this happens RUNNING is set to 0.
    // If thread is blocked and there is no space left to store a pointer
    // to it, hardware status returns mask::ERROR and this method throws -3.
    // If the semaphore specified does not exist in hardware, throws -4.
    void p();
    
    // Unlock command. Executes the up command on semaphore's hardware.
    // As result, increments the semaphore's counter (stored in hardware).
    // If previous value was negative, unblocks next thread from FIFO.
    // The pointer is returned in output::DATA and status is set to mask::DONE
    // and mask::RESUME.
    // If the semaphore specified does not exist in hardware, throws -4. 
    void v();

 private:
    // Identifier taken from semaphore's hardware.
    unsigned int id;

    // Method to abstract commands passed to semaphore's hardware.
    void execute_command(unsigned command, unsigned data = 0);
    // Method to read output::DATA from semaphore's hardware.
    unsigned read_data();
    // Method to read output::STATUS from semaphore's hardware.
    unsigned read_status();
};

#endif /* SEMAPHORE_H_ */
