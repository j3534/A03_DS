/*
* FILE			    : A3.c
* PROJECT	        : Assignment 3 - Sorted Linked Lists and Hash Tables
* PROGRAMMER		: Justin Li
* FIRST VERSION		: 11/16/2026
* DESCRIPTION		:
*	This file contains the code for the program's entire functionality
*/

#include <stdio.h>

#define BUCKET_COUNT 127

struct NamesNode {
	char* name;

	struct NamesNode* next;
};

struct NamesHashTable {
	struct NamesNode* buckets[BUCKET_COUNT];
};

/*
* FUNCTION		    : main
* DESCRIPTION		:
*	The entrypoint of the program
* PARAMETERS		:
*	None
* RETURNS		    :
* 	int 	    	: Whether the execution was successsful
*/
int main() {
	// names.txt is a text file containing 3000 all lowercase full names

	// it is placed in the same directory as this source file
	// so that the relative path "names.txt" can be easily used

	FILE* namesFile = fopen("names.txt", "r");
	if (namesFile == NULL) {
		printf("Error: failed to open names.txt, check that it exists");
		return;
	}

	

	return 0;
}