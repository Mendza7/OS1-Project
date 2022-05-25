/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include "System.h"

IVTEntry* IVTEntry::entries[256];

IVTEntry::IVTEntry(pInterrupt inter,IVTNo number) {

	if((number<0) || (number>255) || event) return;

	System::lock();

	newIntrr = inter;

#ifndef BCC_BLOCK_IGNORE
	OldIntrr = getvect(number);

	setvect(number,newIntrr);
#endif

	entry = number;
	event = 0;
	entries[entry] = this;

	System::unlock();


}

IVTEntry* IVTEntry::getEntry(IVTNo i){return entries[i];}

void IVTEntry::callOldIntrr(){
	OldIntrr();
}

void IVTEntry::signal(){
	if(event)event->signal();
}



IVTEntry::~IVTEntry() {
	System::lock();

	entries[entry] = 0;
#ifndef BCC_BLOCK_IGNORE
	setvect(entry, OldIntrr);
#endif
	event = 0;
	System::unlock();
}

