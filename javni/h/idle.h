/*
 * Idle.h
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_
#include "Thread.h"
#include <IOSTREAM.H>

class Idle: public Thread {
	friend class System;
protected:
	void run();
public:
	Idle(StackSize sz = defaultStackSize, Time ts=1):Thread(sz,ts){}
};

#endif /* H_IDLE_H_ */
