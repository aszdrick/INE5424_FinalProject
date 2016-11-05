/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#include "Semaphore.h"
#include "tests.h"
#include "Thread.h"

int main() {
	tests::creation_destruction();
    tests::max_semaphores();
    tests::destroy_non_empty_semaphore();
    tests::p_decrement();
    tests::v_increment();
    tests::size_init();
    tests::overflow_size_init();
    tests::negative_init();

	return 0;
}
