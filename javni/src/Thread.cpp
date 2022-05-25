
#include "Thread.h"
#define llock asm {pushf;cli;}
#define uunlock asm popf

#include "../h/SYSTEM.h"
#include "SCHEDULE.H"

void Thread::start(){
	//if(myPCB->id == 25) cout<<"Doslo dete"<<endl;
	System::lock();
	myPCB->start();
	System::unlock();
}

void Thread::waitToComplete(){
	myPCB->waitToComplete();
}

Thread::~Thread(){
	waitToComplete();
	System::lock();
	delete myPCB;
	System::unlock();
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	if(PCB::running) return PCB::running->id;
	return 0;
}


Thread* Thread::getThreadById(ID id){
	return PCB::getThreadByID(id);
}

Thread* Thread::clone() const{
	return new Thread();
}


volatile ID childId;
volatile PCB *childPCB;

ID Thread::fork(){
	//syncPrintf("Ulaz u fork\n");


	System::lock();

	Thread* child = 0;
	child = PCB::running->thread->clone();

	if(!child){
		System::unlock();
		return -1;
	}

	childId = child->myPCB->id;
	childPCB = child->myPCB;
	//syncPrintf("%d\n",child->myPCB->id);
	PCB::fork();


	System::unlock();

	return (((PCB::running->id) == childId)?0:childId);

}

void Thread::exit(){
	System::lock();
	//int old = PCB::running->id;
	//cout<<"NIT ID: " << PCB::running->id <<" usla u Thread::exit()"<<endl;
	PCB::unleash();
//	List* discard = new List();
//	int i =0;
//	while(1){
//		PCB* p = Scheduler::get();
//		if(!p)break;
//		if(p->id!=old){
//			discard->insert(p);
//			i--;
//		}
//		i++;
//	}
//	cout<< i<< endl;
//	cout<<"SADRZAJ SCHEDULERA" << endl;
//	discard->printList();
//	PCB* p = discard->getFirst();
//	while(p){
//		Scheduler::put(p);
//		p = discard->getFirst();
//	}
	System::unlock();
	dispatch();

}

void Thread::waitForForkChildren(){
	System::lock();

	//cout<< "Ulazak u waitforForkChildren " << PCB::running->id<<endl<<"READY: ";
	//System::readyThreads->printList();
	if(!PCB::running->waitChildrenSem){
		//syncPrintf("Pravim semafor\n");
		PCB::running->waitChildrenSem = new KernelSem(0);
	}

	System::unlock();
	while(PCB::running->children){
		//syncPrintf("usao u while\n");
		PCB::running->waitChildrenSem->wait(0);
		//syncPrintf("vratio se sa bloka\n");
		PCB::running->children--;
		//syncPrintf("%d\n",PCB::running->children);
	}

}


Thread::Thread(StackSize sz,Time ts){
	System::lock();
	myPCB = new PCB(this,sz,ts);
	System::unlock();
}
