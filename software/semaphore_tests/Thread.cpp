/*
 * Thread.cpp
 *
 *  Created on: Oct 27, 2016
 *      Authors: Marcio Monteiro and Marleson Graf
 */

#include "Thread.h"

unsigned Thread::thread_counter = 0;
Thread* Thread::RUNNING = 0;
