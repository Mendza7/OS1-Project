/*
 * KerSem.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: OS1
 */

#include "KerSem.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init):val(init) {
	blockedOnThis = new List();
}

void KernelSem::signal(){
	System::lock();
	if(val++<0) unblock();
	System::unlock();
}

int KernelSem::wait(Time waitTime){
	System::lock();
	if(--val<0){
		PCB::running->waitTime=waitTime;
		PCB::running->waitFinished=0;
		block();
		//cout<<"blok na semaforu"<<endl;
		System::unlock();
		dispatch();

		if (PCB::running->waitFinished){
			PCB::running->waitTime=0;
			PCB::running->waitFinished=waitTime;

			return 0;
		}
		else{
			return 1;
		}
	}

	System::unlock();
	return 1;
}

void KernelSem::block(){
	PCB::running->blocked = 1;
	blockedOnThis->insert(PCB::running);
	System::blockedThreads->insert(PCB::running);
	if (PCB::running->waitTime)System::waitingRoom->insert(PCB::running);
	PCB::running->sem = this;
	return;
}

void KernelSem::unblock(){

	PCB* p = blockedOnThis->getFirst();
	if (p){

		System::waitingRoom->takePCB(p);
		p->blocked=0;
		p->waitTime=0;
		p->waitFinished=0;
		System::blockedThreads->takePCB(p);
		System::readyThreads->insert(p);
		Scheduler::put(p);
	}
}


KernelSem::~KernelSem() {
	delete blockedOnThis;
}

