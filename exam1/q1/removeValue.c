// removeValue.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "removeValue" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/



void removeValue(DLList L, int value){
	    while(L->curr != NULL){
        if(L->curr->value == value){
            if(L->nitems == 1){
                free(L->curr);
                L->first = L->last = L->curr = NULL;
            }else{
                //at start
                if(L->curr == L->first){
                    DLListNode *tmp = L->curr;
                    L->curr = L->curr->next;
                    L->curr->prev = NULL;
                    L->first = L->curr;
                    free(tmp);
                //at last
                }else if(L->curr == L->last){
                    DLListNode *tmp = L->curr;
                    L->curr = L->curr->prev;
                    L->curr->next = NULL;
                    L->last = L->curr;
                    free(tmp);
                // at middle
                }else{
                    DLListNode *tmp = L->curr;
                    L->curr = L->curr->next;
                    L->curr->prev = tmp->prev;
                    tmp->prev->next = L->curr;
                    free(tmp);
                }
            }
			L->nitems--;
        }else{
            L->curr = L->curr->next;
        }
    }

    L->curr = L->first;
	return;

}



