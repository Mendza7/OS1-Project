/*
 * System.h
 *
 *  Created on: Sep 13, 2021
 *      Author: OS1
 */

#ifndef SRC_SYSTEM_H_
#define SRC_SYSTEM_H_

#include <dos.h>
#include "idle.h"
#include "LIST.h"
#include "Thread.h"

class List;
class System {
public:
	~System();
	static volatile Thread* idleThread;
	static volatile PCB* mainThread;
	static volatile int infinite_active;
	static void init();
	static void restore();
	static void lock();
	static void unlock();
	static void time();
	static int csod(){return context_switch_on_demand;}
	static List* allThreads;
	static List* readyThreads;
	static List* blockedThreads;
	static List* waitingRoom;

private:
	friend void interrupt timer();
	friend void dispatch();
	friend int syncPrintf(const char* f, ...);
	friend class PCB;
	static volatile int lck;
	static volatile int context_switch_on_demand;
	static volatile unsigned counter;

};

#endif /* SRC_SYSTEM_H_ */
