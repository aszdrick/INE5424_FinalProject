// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#ifndef TESTS_H_
#define TESTS_H_

namespace tests {
    // Test to create and destroy semaphore. Tries to create a semaphore
    // and destroy it, if no exception is thrown, test passed.
    void creation_destruction();
    
    // Test the limit of semaphores created. Semaphore's hardware is set to
    // support at most 8 semaphores. So, 8 semaphores are created successfully,
    // and then it tries to created a ninth semaphore. It should rise an 
    // exception with value -1, otherwise, test failed.
    void max_semaphores();

    // Test the decrement of semaphore's value. Initializes a semaphore with 
    // value 1 and realizes 2 p() operations. The resulting value should be -1 
    // and therefore the runnning thread have to be 0, since p() operation set 
    // it to 0 when a thread is blocked. If this condition does not hold, test 
    // fails.
    void p_decrement();
    
    // Test the increment of semaphore's value. Initializes a semaphore with 
    // value 0 and realizes one v() and one p() operations. Ideally, 
    // semaphore's value changes to 1 with v() and then to 0 again, with p().
    // That way, p() operation should not block a thread. If the running thread
    // is different from 0, this condition holds and test passes.
    void v_increment();
    
    // Test the increment of semaphore's value. Initializes a semaphore with 
    // value 0 and realizes one v() and one p() operations. Ideally, 
    // semaphore's value changes to 1 with v() and then to 0 again, with p().
    // That way, p() operation should not block a thread. If the running thread
    // is different from 0, this condition holds and test passes.
    void destroy_non_empty_semaphore();

    // Test the value initialization of semaphore. Initializes a semaphore with
    // value 7. Tries to do 7 calls to p(). Before that, executes another p(). 
    // This one should block the thread. If no exception is raised the first 
    // seven p() calls and before eighth call the running thread is 0, test 
    // passes.
    void size_init();


    // Initializes a semaphore with size 1023. Since value has only 8
    // bits of representation, the number passed to semaphore will be negative.
    // Therefore, when p() is called, the thread should be blocked. If this
    // condition holds, test passses.
    void overflow_size_init();
    
    // Initializes a semaphore with -1. Executes 2 v() operations, ideally changing
    // value of semaphore to 1. Execute a p() operation, if thread is not blocked,
    // test passes.
    void negative_init();
};

#endif /* TESTS_H_ */
