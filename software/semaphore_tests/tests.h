/*
 * Semaphore.cpp
 *
 *  Created on: Oct 31, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#ifndef TESTS_H_
#define TESTS_H_

namespace tests {
	void creation_destruction();
	void max_semaphores();
	void destroy_non_empty_semaphore();
	void p_decrement();
	void v_increment();
	void size_init();
	void overflow_size_init();
	void negative_init();
};

#endif /* TESTS_H_ */
