/*
 * List.h
 *
 *  Created on: Sep 11, 2021
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
#include "PCB.h"
typedef int ID;
class List {
public:

	List():n(0), head(0), tail(0), curr(0){};
	void insert(PCB* pc);
	PCB* getCurr();
	int getSize();
	void next();
	void printList();
	PCB* takePCB(PCB* pc);
	PCB* takePCBbyID(ID id);
	PCB* getFirst();
	~List();

private:
	friend class System;
	struct Node{
		PCB* pcb;
		Node* next;

		Node(PCB* pc):pcb(pc),next(0){}
		~Node(){
			pcb = 0;
			next = 0;
		}
	};
	int n;
	Node *head, *tail, *curr;
};

#endif /* LIST_H_ */
