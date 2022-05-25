/*
 * PCB.h
 *
 *  Created on: Sep 11, 2021
 *      Author: OS1
 */

#ifndef SRC_PCB_H_
#define SRC_PCB_H_
#include "Thread.h"
#include "KerSem.h"
#include "LIST.h"


class PCB {
	public:
		static void wrapper();
		static PCB* running;
		static unsigned allID;
		static Thread* getThreadByID(ID id);
		void waitToComplete();
		ID id;
	private:

		PCB(Thread* t, StackSize sz, Time ts);
		void start();
		static void interrupt fork();
		static void unleash();

		friend class Thread;
		friend class List;
		friend class System;
		friend void interrupt timer();
		friend int userMain(int argc, char** argv);
		friend class KernelSem;
		friend class KernelEv;


		unsigned ss, sp, bp;
		unsigned* stack;
		StackSize stackSize;


		int blocks;
		int blocked,finished,ready;
		Time timeSlice;
		List* waitingForThis;
		Thread* thread;
		PCB* parent;
		int infinite;

		KernelSem* sem;
		int waitTime;
		int waitFinished;
		int children;
		KernelSem* waitChildrenSem;

		~PCB();
};

#endif /* SRC_PCB_H_ */
