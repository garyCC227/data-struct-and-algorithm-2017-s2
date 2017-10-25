// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/
void insert_last(DLList C, int value){
	
	if(C->nitems == 0){
		DLListNode *new = newDLListNode(value);
		C->curr = C->last = C->first = new;
	}else{
		DLListNode *new = newDLListNode(value);
		//ensure C->curr is at last;		
		while(C->curr->next != NULL){
			C->curr = C->curr->next;
		}
		new->prev = C->curr;		
		C->curr->next = new;
		C->curr = new;
		C->last = C->curr;
	}
	C->nitems++;
}

DLList getPeaks(DLList L){

	DLList peaksL = newDLList();
	if(L->nitems <= 3){
		return peaksL;
	}
	// ignore first node	
	L->curr = L->curr->next;
	
	//avoid last node	
	while(L->curr->next != NULL){
		if((L->curr->value > L->curr->next->value) && (L->curr->value > L->curr->prev->value)){
		insert_last(peaksL,L->curr->value);
		}
		L->curr = L->curr->next;
	}

	L->curr = L->first;
	return peaksL;

}



