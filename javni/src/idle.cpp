/*
 * Idle.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "../h/idle.h"

#include <IOSTREAM.H>

#include "../h/SYSTEM.h"



void Idle::run(){
	for (int i =0; i < 30; ++i) {
			for (int k = 0; k<10000; ++k){
				for (int j = 0; j <30000; ++j);
				if(k == 9999){

					k = 1;
					//cout << "idle" << endl;
				}
			}
		}
}
