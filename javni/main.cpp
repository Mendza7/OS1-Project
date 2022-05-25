/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include <stdio.h>

#include "Thread.h"
#include "List.h"
#include "System.h"
#include "semaphor.h"
#include "event.h"
#include "KerEv.h"
#include "stdlib.h"
#define llock asm {pushf;cli;}
#define uunlock asm popf


//TEST BEGIN--------------------------------------------------------------

//TEST END----------------------------------------------------------------




extern int userMain(int argc, char** argv);


class Main : public Thread {
	int argc;
	char **argv;
public:

	Main(int argc, char **argv) : Thread(59999, 0), argc(argc), argv(argv) {
		val =0;
	}

	int val;
	void run() {
		//cout << " usla u main-run" <<endl;
		val = userMain(argc, argv);
	}

	~Main() { waitToComplete(); }
};


int main(int argc, char** argv)
{

	System::init();
	//TEST FORK
	Main* m = new Main(argc,argv);
	m->start();
	delete m;
	//TEST REGULAR
	//int k = userMain(argc, argv);

	System::restore();

	//TEST FORK
	//syncPrintf("%d",m->val);
	return m->val;

	//TEST REGULAR
	//return k;
}


