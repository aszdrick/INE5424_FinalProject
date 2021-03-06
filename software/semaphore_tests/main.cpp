// Copyright (C) 2016 Marcio Monteiro, Marleson Graf

#include "Semaphore.h"
#include "tests.h"
#include "Thread.h"

int main() {
    tests::creation_destruction();
    tests::max_semaphores();
    tests::p_decrement();
    tests::v_increment();
    tests::destroy_non_empty_semaphore();
    tests::size_init();
    tests::overflow_size_init();
    tests::negative_init();

    return 0;
}
