/*
 * KernelEv.h
 *
 *  Created on: Sep 25, 2021
 *      Author: OS1
 */

#ifndef SRC_KERNELEV_H_
#define SRC_KERNELEV_H_

#include "IVTEntry.h"


typedef int IVTNo;
class PCB;
class IVTEntry;

class KernelEv {
	public:
		KernelEv(IVTNo i);
		virtual ~KernelEv();
		void wait();
		void signal();
	private:
		IVTNo num;
		PCB* source;
		int cap;
		IVTEntry* IVTpoint;

};

#endif /* SRC_KERNELEV_H_ */
