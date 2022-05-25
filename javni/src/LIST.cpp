/*
 * List.cpp
 *
 *  Created on: Sep 11, 2021
 *      Author: OS1
 */

#include "../h/LIST.h"
#include "PCB.h"

extern int syncPrintf(const char* f, ...);

int List::getSize(){return n;}

void List::insert(PCB* pc){
	if (pc==0) return;
	Node* node = new Node(pc);
	n++;
	if (head ==0 ){
		head = node;
	}
	else tail->next = node;
	tail = node;
}

void List::next(){
	if (curr==0) return;
	curr = curr->next;
}

void List::printList(){
	Node* temp = head;
	//syncPrintf("READY: ");
	while(temp){
		syncPrintf("%d ", temp->pcb->id);

		temp = temp->next;
	}
	syncPrintf("\n");
}

PCB* List::takePCB(PCB* pc){
	if (!n || !pc) return 0;
	Node* temp = head;
	Node* prev= 0;
	Node* next = 0;
	if (temp){
		next = temp->next;
	}

	while(temp){
		if (temp->pcb == pc){
			PCB* pcb = temp->pcb;
			if (prev==0){
				head = head->next;
				if(!head){
					tail = head;
				}
				temp->next = 0;
			}
			else{
				if(!temp->next){
					tail = prev;
					tail->next= 0;
				}
				else{
					prev->next = next;
					temp->next = 0;
				}
			}
			n--;
			delete temp;
			return pcb;
		}

		prev = temp;
		temp = next;
		if (temp){
			next = temp->next;
		}
	}
	return 0;
}

PCB* List::takePCBbyID(ID id){
	if (!n || !id) return 0;
		Node* temp = head;
		Node* prev= 0;
		Node* next = 0;
		if (temp){
			next = temp->next;
		}

		while(temp){
			if ((temp->pcb)->id == id){
				PCB* pcb = temp->pcb;
				if (prev==0){
					head = head->next;
					if(!head){
						tail = head;
					}
					temp->next = 0;
				}
				else{
					if(!temp->next){
						tail = prev;
						tail->next= 0;
					}
					else{
						prev->next = next;
						temp->next = 0;
					}
				}
				n--;
				delete temp;
				return pcb;
			}

			prev = temp;
			temp = next;
			if (temp){
				next = temp->next;
			}
		}
		return 0;
}

PCB* List::getCurr(){
	if (curr){
		return curr->pcb;
	}
	return 0;
}

PCB* List::getFirst(){
	if(head){
		Node* temp = head;
		head = head->next;
		n--;
		temp->next = 0;
		PCB* p = temp->pcb;
		delete temp;
		return p;
	}
	return 0;
}


List::~List(){

	while(head){
		curr = head;
		head = head->next;
		delete curr;
	}

	head = tail = curr = 0;
}
