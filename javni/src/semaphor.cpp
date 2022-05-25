/*
 * Semaphore.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */

#include "../h/semaphor.h"

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);
}

void Semaphore::signal(){
	myImpl->signal();
	return;
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::val()const{
	return myImpl->val;
}

Semaphore::~Semaphore()
{
	delete myImpl;
}
