/*
 * KerSem.h
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

class List;
#include "SYSTEM.H"

class KernelSem {
public:
	KernelSem(int init);
	int wait(Time waitTime);
	void signal();
	virtual ~KernelSem();
	List* blockedOnThis;
private:
	void unblock();
	void block();
	friend class Semaphore;
	int val;
};

#endif /* KERSEM_H_ */
