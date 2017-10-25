/* listIteratorG.c ... Generic List Iterator implementation
Written by linchen CHEN
last update: 29 August 2017

 problem:
 1. my cursor will never sit before the head elems 

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"
#define BEFORE_HEAD 1		//case A: cursor sit before IteratorGRep->elems 
#define AFTER_HEAD 0		//case B: cursor sit after IteratorGRep->elems
							//only consider cursor will set before the IteratorGRep->elems 
							//as IteratorGRep->elems == head
/*
0 for invalid call, precondition: 1 for next()
								  2 for prev()
								  3 for findNext()
								  4 for findPrev()
note: conditioner will be reassigned to INVALID to all the function call exclude next(),prev(),findNext(),findPrev()
*/
#define INVALID 0		
#define NEXT 1
#define PREV 2
#define FINDNEXT 3
#define FINDPREV 4

typedef struct Node {
 void *value;
 struct Node *next;
 struct Node *prev;
} Node;

typedef struct IteratorGRep {
	Node *elems;	
	Node *head;		// head of the list
	Node *tail;		// tail of the list
	ElmCompareFp cmpFp;		
	ElmNewFp newFp;
	ElmFreeFp freeFp;
	int nitems;		//defind the number of elements in this struct
	int headCounter;	//edge case to determine the cursor is in case A or B
						//BEFORE_HEAD 1 case---A   AFTER_HEAD 0---case B
	//conditioner for delete and set function
	int conditioner;
} IteratorGRep;		

// code-----------------------------------------------
// get a new list;
IteratorG IteratorGNew(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp){
	IteratorGRep *news;
	news = malloc(sizeof(IteratorGRep));
	assert(news != NULL);

	news->elems = news->head= news->tail =NULL;
	news->cmpFp = cmpFp;
	news->newFp = newFp;
	news->freeFp = freeFp;
	news->nitems = 0;
	news->headCounter = AFTER_HEAD;// by default, the cursor sit after news->elems
	news->conditioner = INVALID;

 	return news;
}
/*insert a node into list iterator, Node will be inserted before cursor.
case 1: empty list, the newNode will become the solo elems in list.
case 2: solo Node, will consider two situation for cursor setting: case A and case B
case 3:	more than 1 Node:
		1)cursor is at head, will consider two situation for cursor: case A and B
		2)cursor is at last, the newNode will become the last , and new->next will point to NULL.
		3)normal insert :the newNode will be inserted before the cursor
*/
int add(IteratorG it, void *vp){//good
	assert(it != NULL);

	Node *new;
	if ((new = malloc(sizeof(Node))) == NULL) { 
		it->conditioner = INVALID;
		return 0; 
	}
	new->value = it->newFp(vp);
	//case 1: insert when empty list
	if(it->nitems == 0){
		it->elems = new;
		new->next = new->prev = NULL;
		it->head = it ->tail = new;
		//regular action
		//set conditioner to invalid
		it->conditioner = INVALID;
		it->nitems++;
		return 1;
	//case 2:solo Node in list
	}else if(it->nitems == 1){
		//cursor is in case A: sit before it->elems
		if(it->headCounter == BEFORE_HEAD){
			new->next = it->elems;
			it->elems->prev = new;
			new->prev = NULL;
			it->head = new;
			it->tail = it->elems;
			//turn cursor setting to case B(default)
			it->elems = new;
			it->headCounter = AFTER_HEAD;
		//cursor is in case B:sit after it->elems
		}else if(it->headCounter == AFTER_HEAD){
			new->prev = it->elems;
			it->elems->next = new;
			new->next = NULL;
			it->elems = new;
			it->tail = new;
		}else{
			// insert fail
			return 0;
		}
		//regular action
		//set conditioner to invalid
		it->conditioner = INVALID;
		it->nitems++;
		return 1;
	//case 3:more than 1 Node
	}else{
		//2) cursor at last
		if(it->elems == it->tail){
			new->prev = it->elems;
			it->elems->next = new;
			it->elems = new;
			new->next = NULL;
			it->tail = new;
		//1) cursor at head
		}else if(it->elems == it->head){
			//cursor is in case A: sit before it->elems
			if(it->headCounter == BEFORE_HEAD){
				new->next = it->elems;
				it->elems->prev = new;
				new->prev = NULL;
				it->head = new;
				//turn cursor setting to case B(default)
				it->elems = new;
				it->headCounter = AFTER_HEAD;
			//cursor is in case B:sit after it->elems
			}else if(it->headCounter == AFTER_HEAD){
				new->prev = it->elems;
				it->elems->next->prev = new;
				new->next = it->elems->next;
				it->elems->next = new;
				it->elems = new;
			}else{
				//insert fail
				return 0;
			}
		}else{
			//3)normal insert: cursor in middle
			new->prev = it->elems;
			it->elems->next->prev = new;
			new->next = it->elems->next;
			it->elems->next = new;
			it->elems = new;
		}
		//regular action
		//set conditioner to invalid
		it->conditioner = INVALID;
		it->nitems++;
		return 1;
	}
	
	// insert fail if function come to here
	//set conditioner to invalid
	it->conditioner = INVALID;
	return 0;
}

int hasNext(IteratorG it){//good
	
	int count = 0;
	Node *curr;
	for(curr = it->head;curr != NULL; curr = curr->next){
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf(stderr, "Invalid forward link into node\n");
			return 0;
		}
		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf(stderr, "Invalid backward link into node\n");
			return 0;
		}
		count++;
	}
	if (count != it->nitems) {
		fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n",
		count, it->nitems);
		return 0;
	}else{
		//set conditioner to invalid
		it->conditioner = INVALID;
		return 1;
 	}
}

int hasPrevious(IteratorG it){//test?
 int count = 0;
 Node *curr;
 for(curr = it->tail;curr != NULL; curr = curr->prev){
	 if (curr->prev != NULL && curr->prev->next != curr) {
		 fprintf(stderr, "Invalid forward link into node \n");
		 return 0;
	 }
	 if (curr->next != NULL && curr->next->prev != curr) {
		 fprintf(stderr, "Invalid backward link into node \n");
		 return 0;
	 }
	 count++;
 }
 if (count != it->nitems) {
	 fprintf(stderr, "backward count mismatch; counted=%d, nitems=%d\n",
			 count, it->nitems);
	 return 0;
 }else{
	 //set conditioner to invalid
	 it->conditioner = INVALID;
	 return 1;
 }
}
// advance the cursor and return a pointer that point to next value(next to cursor), if no next value then return NULL
//consider two case : cursor setting A and B  
void * next(IteratorG it){//good
	void *ptr = NULL;
	//when it->elems at head, consider cursor setting
	if(it->elems == it->head){
		//when cursor setting is case B
		if(it->headCounter == BEFORE_HEAD){
			ptr = it->elems->value;
			// set cursor back to case B
			it->headCounter = AFTER_HEAD;
		//cursor setting is case A	
		}else if(it->headCounter == AFTER_HEAD){
			//cursor at last, no next value so return NULL
			ptr = it->elems->next->value;
			it->elems = it->elems->next;
		}
	}else{
		if(it->elems == it->tail){
			//set conditioner to NEXT 
			it->conditioner = NEXT;
			return NULL;
		}
		ptr = it->elems->next->value;
		it->elems = it->elems->next;
	}
	//set conditioner to NEXT 
	it->conditioner = NEXT;
	return ptr;
}

// retreat the cursor and return a pointer that point to previous value(previous to cursor), if no next value then return NULL
//consider two case : cursor setting A and B 
void * previous(IteratorG it){//good
	void *ptr = NULL;
	//when it->elems at head, consider cursor setting
	if(it->elems == it->head){
		//cursor at head, invaild call prev()
		if(it->headCounter == BEFORE_HEAD){
			//set conditioner to PREV 
			it->conditioner = PREV;
			return NULL;
		}else if(it->headCounter == AFTER_HEAD){
			ptr = it->elems->value;
			//cursor move to the start
			it->headCounter = BEFORE_HEAD;		
		}
	// others are all normal case B	
	}else{
		ptr = it->elems->value;
		it->elems = it->elems->prev;
	}
	//set conditioner to PREV 
	it->conditioner = PREV;
	return ptr;
}
/* 
---check precondition is valid or not;
---then deletion situation:
---deleted element is in:
---1)first
---2)last
---3)middle
---sucessfully deletion will return 1, otherwise 0(invalid call delete function)
---and cursor will stay at same position

Deletes from the list iterator the last value that was returned by next or previous or findNext or findPrevious. 
Precondition: A call to delete must be IMMEDIATELY preceded by a successful call to one of 
next or previous or findNext or findPrevious.
*/
int delete(IteratorG it){//good
	if(it->conditioner == INVALID){
		fprintf(stderr,"invalid delate call\n");
		return 0;
	}else{
		//next() was called
		if(it->conditioner == NEXT){	
			//it->elems is the wanted delete elems
			//delete tail elems
			if(it->elems == it->tail){
				Node *tmp = it->elems;
				it->elems = it->elems->prev;
				it->elems->next = NULL;
				it->tail = it->elems;
				free(tmp);
			//delete head elems
			}else if(it->elems == it->head){
				Node *tmp = it->elems;
				it->elems = it->elems->next;
				it->head = it->elems;
				free(tmp);
				//set cursor at head of list
				it->headCounter = BEFORE_HEAD;
			//delete middle elems
			}else{
				Node *tmp = it->elems;
				it->elems->prev->next = it->elems->next;
				it->elems->next->prev = it->elems->prev;
				it->elems = it->elems->prev;
				free(tmp); 
			}
		//prev() was called
		}else if(it->conditioner == PREV){
			//wanted deleted elemens is it->elems->next
			//it->elems at tail before called
			if(it->elems->next == it->tail){
				Node *tmp = it->elems->next;
				it->elems->next = NULL;
				it->tail = it->elems;
				free(tmp);
			//it->elems at head before called will consider cursor setting case A or B
			}else if(it->elems == it->head){
				if(it->headCounter == AFTER_HEAD){
					Node *tmp = it->elems->next;
					it->elems->next->next->prev = it->elems;
					it->elems->next = it->elems->next->next;					
					free(tmp);
				}else if(it->headCounter == BEFORE_HEAD){
					Node *tmp = it->elems;
					it->elems = it->elems->next;
					it->elems->prev = NULL;
					it->head = it->elems;
					free(tmp);
					//keep cursor at head of list
				}
			//it->elems at middle before called
			}else{
				Node *tmp = it->elems->next;
				it->elems->next->next->prev = it->elems;
				it->elems->next = it->elems->next->next;
				free(tmp);
			}
		}else if(it->conditioner == FINDNEXT){
			if(it->elems->next == it->tail){
				Node *tmp = it->elems->next;
				it->elems->next = NULL;
				it->tail = it->elems;
				free(tmp);
			}else{			
				Node *tmp = it->elems->next;
				it->elems->next->next->prev = it->elems;
				it->elems->next = it->elems->next->next;
				free(tmp);
			}
		}else if(it->conditioner == FINDPREV){
			if(it->elems == it->head){
				Node *tmp = it->elems;
				it->elems = it->elems->next;
				it->elems->prev = NULL;
				it->head = it->elems;
				free(tmp);
				//set cursor to head of list
				it->headCounter = BEFORE_HEAD;
			}else{
				Node *tmp = it->elems;
				it->elems->next->prev = it->elems->prev;
				it->elems->prev->next = it->elems->next;
				it->elems = it->elems->prev;
				free(tmp);
			}
		}
		// set conditioner to invalid
		it->conditioner = INVALID;
		it->nitems--;
		return 1;
	}
}

int set(IteratorG it, void * vp){// good
	if(it->conditioner == INVALID){
		fprintf(stderr,"invalid set call\n");
		return 0;
	}else{
		//next() was called
		if(it->conditioner == NEXT){	
			//it->elems is the wanted replace elems
			it->elems->value = it->newFp(vp);			
		//prev() was called
		}else if(it->conditioner == PREV){
			//wanted replace elemens is it->elems->next
			//it->elems at tail before called
			if(it->elems->next == it->tail){
				it->elems->next->value = it->newFp(vp);
			//it->elems at head before called will consider cursor setting case A or B
			}else if(it->elems == it->head){
				if(it->headCounter == AFTER_HEAD){
					it->elems->next->value = it->newFp(vp);
				}else if(it->headCounter == BEFORE_HEAD){
					it->elems->value = it->newFp(vp);
					//keep cursor at head of list
				}
			//it->elems at middle before called
			}else{
				it->elems->next->value = it->newFp(vp);
			}
		//findNext() called
		}else if(it->conditioner == FINDNEXT){//test 1)*vp->next in middle(normal case) 2) *vp->next = tail
			// wanted to replace is it->elems->next
			it->elems->next->value = it->newFp(vp);
		//findprev() called
		}else if(it->conditioner == FINDPREV){//test 1)head is prev, 2)normal case in middle
			it->elems->value = it->newFp(vp);
		}
		// set conditioner to invalid
		it->conditioner = INVALID;
		return 1;
	}
}

/* 
find the specific value in list, if no contains in list will return NULL
cursor will sit after the value *vp
*/
void * findNext(IteratorG it, void * vp){ //good
	Node *curr = it->head;
	
	while(curr != NULL){
		if((it->cmpFp(vp,curr->value)) == 0){
			// if curr->next == NULL will return NULL, cursor does not move
			if(curr->next == NULL){
				//set conditioner to FINDNEXT 
				it->conditioner = FINDNEXT;
				return NULL;
			}
			void *ptr = curr->next->value;
			it->elems = curr; // set cursor position after it->elems, since cursor sit after it->elems by default
			//set conditioner to FINDNEXT 
			it->conditioner = FINDNEXT;
			return ptr;
		}
		curr = curr->next;
	}
	// if *vp is not in list, will return NULL 
	//set conditioner to FINDNEXT 
	it->conditioner = FINDNEXT;
	return NULL;
}

/* 
find the specific value in list, if no contains in list will retun NULL
cursor will sit before the value *vp
*/
void * findPrevious(IteratorG it, void * vp){ //good
	Node *curr = it->head;
	
	while(curr != NULL){
		if((it->cmpFp(vp,curr->value)) == 0){
			// if curr->prev == NULL will return NULL, cursor does not move
			if(curr->prev == NULL){
				//set conditioner to FINDPREV 
				it->conditioner = FINDPREV;
				return NULL;
			}
			void *ptr = curr->prev->value;
			it->elems = curr->prev; // set cursor position before it->elems, since cursor sit after it->elems by default
			//set conditioner to FINDPREV 
			it->conditioner = FINDPREV;
			return ptr;
		}
		curr = curr->next;
	}
	// if *vp is not in list, will return NULL
	//set conditioner to FINDPREV 
	it->conditioner = FINDPREV; 
	return NULL;
}

//reset it->elems to the start of list
void reset(IteratorG it){ //good
	//move cursor after the head elemens
	it->elems = it->head;
	//move cursor to the head of list
	it->headCounter = BEFORE_HEAD;
	//set conditioner to invalid
	it->conditioner = INVALID;
}

//Deletes all the nodes in it and frees associated memory.
void freeIt(IteratorG it){//good
	Node *curr;
	
	while(it->head != NULL){
		curr = it->head;
		it->head = curr->next;
		it->freeFp(curr->value);
		it->freeFp(curr);
	}
	it->freeFp(it);
}
/*
void putlist(IteratorG it){
	
	Node *curr = it->head;
	int *ptr = (int *)curr->value;
	printf("%d,",*ptr);

	curr = curr->next;
	int *ptr2 = (int *)curr->value;
	printf("%d,",*ptr2);
	
	curr = curr->next;
	ptr2 = (int *)curr->value;
	printf("%d,",*ptr2);
	
	curr = curr->next;
	ptr2 = (int *)curr->value;
	printf("%d,",*ptr2);
	printf("\n");
	
	
	
	Node *curr = it->head;
	int *ptr = NULL;
	while(curr != NULL){
		ptr = (int *)curr->value;
		printf("%d,",*ptr);
		curr = curr->next;
	}
	printf("\n");

	Node *cursor_bf = it->elems;
	ptr = (int *)cursor_bf->value;
	printf("it->elem is %d,",*ptr);
	printf("\n");
	
}
*/


