// File: event.h
#ifndef _event_h_
#define _event_h_

#include "KerEv.h"

typedef void interrupt (*pInterrupt)(...);


#define PREPAREENTRY(entry,callOld)\
void interrupt interrRout##entry(...);\
IVTEntry Ientry##entry (interrRout##entry,entry);\
void interrupt interrRout##entry(...){\
	Ientry##entry.signal();\
	if(callOld)Ientry##entry.callOldIntrr();\
}


typedef int IVTNo;
class KernelEv;

class Event {
	public:
		Event (IVTNo ivtNo);
		~Event ();
		void wait ();
	protected:
		friend class KernelEv;
		void signal(); // can call KernelEv
	private:
		KernelEv* myImpl;
};
#endif
