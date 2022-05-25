/*
 * System.cpp
 *
 *  Created on: Sep 13, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include <IOSTREAM.H>

#include "../h/SYSTEM.h"
#include "SCHEDULE.H"


extern void tick();
extern int syncPrintf(const char*, ...);

volatile unsigned tss, tsp, tbp, old;
unsigned oldTimerOFF, oldTimerSEG;
volatile int System::lck = 0;
volatile int System::context_switch_on_demand = 0;
volatile unsigned System::counter = defaultTimeSlice;
volatile int System::infinite_active=0;

List* System::allThreads = new List();
List* System::readyThreads = new List();
List* System::blockedThreads = new List();
List* System::waitingRoom = new List();

volatile Thread* System::idleThread = 0;
volatile PCB* System::mainThread = 0;

void System::init(){
	//timer
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es, ax

		//pamti staru
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF,ax

		//postavljamo novu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		//postavljamo staru na 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}

	asm cli
	mainThread = new PCB(0,defaultStackSize,defaultTimeSlice);
	mainThread->ready=1;
	//cout <<"MAIN  "<<mainThread->id <<endl;



	idleThread = new Idle();
	idleThread->start();
	//cout <<"IDLE  "<< idleThread->myPCB->id<<endl<<endl;
	//System::readyThreads->printList();
	PCB::running = (PCB*)mainThread;
	Scheduler::put(PCB::running);

	asm sti

#endif


}

void System::restore(){

#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es, ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif

	delete System::allThreads;
	delete System::blockedThreads;
	delete System::readyThreads;
}



void System::lock(){
	lck++;
}

void System::unlock(){
	if( --lck ==0 && context_switch_on_demand)
		dispatch();
}


void System::time(){
	List::Node* temp = System::waitingRoom->head;
	List::Node* prev =0;

	while(temp){
		if(--(temp->pcb->waitTime)==0){
			PCB* p = temp->pcb;
			p->waitFinished=1;
			p->blocked = 0;
			p->sem->blockedOnThis->takePCB(p);
			p->sem = 0;
			System::blockedThreads->takePCB(p);
			Scheduler::put(p);

			if(!prev) {
				System::waitingRoom->head = temp->next;
				temp = temp->next;
			}
			else{
				prev->next = temp->next;
				if(!temp->next)System::waitingRoom->tail = prev;
				else temp->next = 0;
				temp->pcb =0;
				delete temp;
				temp = prev->next;
			}

		}
		else{
			prev = temp;
			temp= temp->next;
		}
	}

}


void dispatch(){

#ifndef BCC_BLOCK_IGNORE
	asm {pushf; cli;}
	System::context_switch_on_demand = 1;
	timer();
	asm {popf}
#endif

}

void interrupt timer(){


	if (!System::csod()){
		tick();
		System::time();


		asm int 60h;
	}

	if(((--System::counter<=0) && !System::infinite_active) || System::context_switch_on_demand){
		if(System::lck==0){

			System::context_switch_on_demand=0;

			asm{
				mov tss, ss
				mov tsp, sp
				mov tbp, bp
			}

			PCB::running->ss = tss;
			PCB::running->sp = tsp;
			PCB::running->bp = tbp;

			old = PCB::running->id;
			if(!PCB::running->blocked && !PCB::running->finished && PCB::running->ready && (PCB::running->id != 1)){
				System::readyThreads->insert(PCB::running);
				Scheduler::put((PCB*)PCB::running);
			}
			PCB::running = Scheduler::get();





			System::readyThreads->takePCB(PCB::running);


			if (!PCB::running || PCB::running->finished){

				PCB::running = System::idleThread->myPCB;
			}


			if (!PCB::running->infinite){
				System::counter = PCB::running->timeSlice;
				System::infinite_active=0;
			}
			else{
				System::infinite_active=1;
				System::counter = 1;
			}


			tss = PCB::running->ss;
			tsp = PCB::running->sp;
			tbp = PCB::running->bp;

			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			//System::lock();
			//cout<<"promena konteksta sa: "<< old <<" na: " << PCB::running->id<<endl;
			//asm cli;
			//System::unlock();

		}
		else {
			System::context_switch_on_demand=1;
		}

	}
}
