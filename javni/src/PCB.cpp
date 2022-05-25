/*
 * PCB.cpp
 *
 *  Created on: Sep 11, 2021
 *      Author: OS1
 */
#include <DOS.H>

#include "PCB.h"

#include "../h/SYSTEM.h"
#include "SCHEDULE.H"
#include "Thread.h"

extern int syncPrintf(const char*, ...);

PCB* PCB::running = (PCB*)(0);
unsigned PCB::allID=0;

PCB::PCB(Thread* t, StackSize sz, Time ts):ss(0),sp(0),bp(0),stack(0),ready(0),blocked(0),finished(0),timeSlice(ts) {

	id = allID++;
	thread = t;
	blocks = 0;
	children = 0;
	sem = 0;
	waitChildrenSem = 0;
	parent= 0;
	waitTime = 0;
	waitFinished = 0;

	if ((sz>60000) || (sz<100)){
		sz=defaultStackSize;
	}
//	if(sz>defaultStackSize)
//		sz = defaultStackSize;
	this->stackSize = sz/sizeof(unsigned);


	if (timeSlice>0) infinite = 0;
	else infinite = 1;

	waitingForThis = new List();

	stack = new unsigned[stackSize];

	stack[stackSize-1] = 0x200;

#ifndef BCC_BLOCK_IGNORE

	stack[stackSize-2] = FP_SEG(&PCB::wrapper);
	stack[stackSize-3] = FP_OFF(&PCB::wrapper);
	stack[stackSize-12] = 0;
	//PSW,wrSEG,wrOFF,ax,bx,cx,dx...

	sp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);
	bp = FP_OFF(stack + stackSize - 12);

#endif

	if(t)System::allThreads->insert(this);
	//System::allThreads->printList();
}

void PCB::start(){
	if (!ready){
		ready = 1;
		if(this->id!=1){
			System::readyThreads->insert(this);
			Scheduler::put(this);
		}
	}
}


void PCB::wrapper(){

	if (running->thread)running->thread->run();
	unleash();
}

void PCB::unleash(){
	System::lock();
		running->finished = 1;
		running->blocked = 1;
		System::readyThreads->takePCB(running);
		PCB* pc;
		pc = running->waitingForThis->getFirst();
		while(pc){
				pc->blocks--;
				if (pc->blocks==0){
					pc->blocked = 0;
					System::readyThreads->insert(pc);
					Scheduler::put(pc);
				}
				pc = running->waitingForThis->getFirst();
				}
		if (running->parent && running->parent->waitChildrenSem){
			running->parent->waitChildrenSem->signal();
		}

	System::unlock();
	dispatch();
}


Thread* PCB::getThreadByID(ID id){
	PCB* pcb = System::allThreads->takePCBbyID(id);

	if(pcb) return pcb->thread;
	return 0;
}

void PCB::waitToComplete(){
	System::lock();

	if(finished || (running == this) || (running == System::idleThread->myPCB)){
		System::unlock();
		return;
	}
	//syncPrintf("%s %d ubacena-> %d\n", "U wait od ",this->id, running->id);
	waitingForThis->insert(running);

	running->blocks++;
	running->blocked = 1;

	System::readyThreads->takePCB(running);
	System::blockedThreads->insert(running);
	System::unlock();
	dispatch();


}


unsigned offsetP;
unsigned offsetC;
unsigned *spP;
unsigned *spC;

unsigned tempbp;
unsigned tempss;

extern volatile PCB *childPCB;


void interrupt PCB::fork(){

	System::lock();

	//syncPrintf("Ulaz u PCB::fork");
	//syncPrintf("%d\n",childPCB->id);

	delete[] childPCB->stack;
	childPCB->stack = new unsigned[running->stackSize];
	childPCB->timeSlice = running->timeSlice;
	childPCB->stackSize = running->stackSize;
	childPCB->parent = running;
	running->children++;
	//syncPrintf("%d ->DECE: %d\n",running->id,running->children);

	memcpy(childPCB->stack, running->stack, childPCB->stackSize*sizeof(unsigned));

#ifndef BCC_BLOCK_IGNORE
	asm{

		mov tempss,ss
		mov tempbp,bp

	}

	childPCB->ss = FP_SEG(childPCB->stack);
	offsetC = FP_OFF(childPCB->stack);
	offsetP = FP_OFF(running->stack);

	childPCB->bp = childPCB->sp = tempbp - offsetP+offsetC;

	offsetP = tempbp;
	offsetC = childPCB->bp;


	for(;;){
		spP = (unsigned*)MK_FP(tempss,offsetP);
		spC = (unsigned*)MK_FP(childPCB->ss, offsetC);
		if(!(*spP)){
			*spC = 0;
			break;
		}
		*spC = *spP - offsetP + offsetC;
		offsetP = *spP;
		offsetC = *spC;

	}
#endif

	//syncPrintf("zavrseno kopiranje\n");
	//syncPrintf("%d %d %d\n",childPCB->id,childPCB->ready, childPCB->parent->id);
	//syncPrintf("RUNNING: %d\n",running->id);
	//syncPrintf("OTAC: %d\n",childPCB->parent->id);
	//childPCB->ready = 0;
	childPCB->thread->start();
	//syncPrintf("zavrsen start\n");
	System::unlock();
}


PCB::~PCB(){
	System::allThreads->takePCB(this);
	System::readyThreads->takePCB(this);
	System::blockedThreads->takePCB(this);
	delete waitingForThis;
	thread = 0;
	sem = 0;
	parent = 0;
	delete waitChildrenSem;
	delete[] stack;
}

