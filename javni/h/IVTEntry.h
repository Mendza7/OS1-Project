/*
 * IVTEntry.h
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#ifndef SRC_IVTENTRY_H_
#define SRC_IVTENTRY_H_

#include "System.h"
#include "KerEv.h"

typedef int IVTNo;
typedef void interrupt (*pInterrupt)(...);


class IVTEntry {
public:
	IVTEntry(pInterrupt inter,IVTNo number);
	void signal();
	void callOldIntrr();
	~IVTEntry();
	 static IVTEntry* getEntry(IVTNo i);
private:
	friend class KernelEv;

	static IVTEntry* entries[256];
	KernelEv* event;

	pInterrupt newIntrr, OldIntrr;
	IVTNo entry;
};

#endif /* SRC_IVTENTRY_H_ */
