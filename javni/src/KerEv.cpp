/*
 * KernelEv.cpp
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#include "../h/KerEv.h"
#include "SCHEDULER.H"

KernelEv::KernelEv(IVTNo i):cap(0),num(i) {
	System::lock();

		source = PCB::running;
		IVTpoint = IVTEntry::getEntry(num);
		IVTpoint->event=this;

	System::unlock();

}

void KernelEv::wait(){
	System::lock();

	if((source == PCB::running) && cap){
		cap = 0;
		System::unlock();
		return;
	}
	else{

		System::readyThreads->takePCB(source);
		System::blockedThreads->insert(source);
		source->blocked = 1;

		System::unlock();
		dispatch();
	}
}


void KernelEv::signal(){
	System::lock();
	if(source->blocked){

			System::readyThreads->insert(source);
			System::blockedThreads->takePCB(source);
			source->blocked = 0;
			Scheduler::put(source);

	}
	else{
		cap = 1;
	}
	System::unlock();
}

KernelEv::~KernelEv() {
	System::lock();
		if(IVTpoint)delete IVTpoint;
		source = 0;
	System::unlock();
}

