/*
 * semaphor.h
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */

// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_
#include "KerSem.h"
typedef unsigned int Time;

class Semaphore {
	public:
	 	 Semaphore (int init=1);
		 virtual ~Semaphore ();
		 virtual int wait (Time maxTimeToWait);
		 virtual void signal();
		 int val () const; // Returns the current value of the semaphore
	private:
		 KernelSem* myImpl;
};
#endif
