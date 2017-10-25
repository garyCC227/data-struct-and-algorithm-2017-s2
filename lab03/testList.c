// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"

void TestAll();

int main(int argc, char *argv[])
{
	
	TestAll();

	// TODO: more tests needed here
	return 0;
}

// test opreation 'i'
void TestBefore(){
		
	DLList myList;
	myList = newDLList();
	printf("test opreation ' i ' -- insert infront of current line\n");

	// insert in front of NULL	
	DLListBefore(myList,"first insert when list is empty");
	assert(validDLList(myList));
	putDLList(stdout,myList);

	// insert in front of a element in list(at start)
	DLListBefore(myList,"second insert when have one element in list");
	assert(validDLList(myList));
	putDLList(stdout,myList);
	//insert when curr is last
	DLListMove(myList,1);
	DLListBefore(myList,"third insert");
	putDLList(stdout,myList);
	assert(validDLList(myList));
	putDLList(stdout,myList);
	//insert when curr is in middle
	DLListMove(myList,1);
	DLListBefore(myList,"fourth insert");
	assert(validDLList(myList));
	putDLList(stdout,myList);
	
	freeDLList(myList);
	printf("You've passed opreation 'i' test!!!!!\n");
}


void TestAfter(){
		DLList myList;
	myList = newDLList();

	printf("test opreation ' a ' -- insert after current line\n");
	
	//insert when list is empty
	DLListAfter(myList,"first insert");
	assert(validDLList(myList));
	putDLList(stdout,myList);

	//insert when list contains one element
	DLListAfter(myList,"2nd insert");
	assert(validDLList(myList));
	putDLList(stdout,myList);

	//insert when *curr is in the middle of the list
	DLListMove(myList,-1);
	DLListAfter(myList,"3nd insert");
	assert(validDLList(myList));
	putDLList(stdout,myList);

	free(myList);
	printf("You've passed\n ");
}


void TestDelete(){
	DLList myList;
	myList = newDLList();

	printf("test opreation ' d ' -- delete current line\n");
	printf("List state before operation:\n");
	
	//delete when list is empty
	 DLListDelete(myList);
	 assert(validDLList(myList));

	 //delete when list contain one element
	 DLListBefore(myList,"one element delete test");
	 putDLList(stdout,myList);
	 DLListDelete(myList);
	 assert(validDLList(myList));
	 putDLList(stdout,myList);

	 //delete the last element
	 DLListBefore(myList,"2nd");
	 DLListBefore(myList,"2nd +1");
	 DLListMove(myList,1);
	 putDLList(stdout,myList);
	 DLListDelete(myList);
	 assert(validDLList(myList));
	 putDLList(stdout,myList);
	
	 //delete when list contain >2 elements
	 //2 element in this case (delete at start)
	 DLListBefore(myList,"3rd");
	 DLListBefore(myList,"3rd +1");
	 putDLList(stdout,myList);
	 DLListDelete(myList);
	 assert(validDLList(myList));
	 putDLList(stdout,myList);
	 
	 // delete when list has 3 element and curr is in middle
	 DLListBefore(myList,"4th");
	 DLListBefore(myList,"4th +1 ");
	 DLListMove(myList,1);
	 putDLList(stdout,myList);
	 DLListDelete(myList);
	 assert(validDLList(myList));
	 putDLList(stdout,myList);

	 free(myList);
	 printf("You've passed\n");
}

void TestAll(){
	TestBefore();
	TestAfter();
	TestDelete();

	printf("You have a fantastic code!!!! \n ");
}
