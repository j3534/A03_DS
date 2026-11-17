/*
* FILE			    : A3.c
* PROJECT	        : Assignment 3 - Sorted Linked Lists and Hash Tables
* PROGRAMMER		: Justin Li
* FIRST VERSION		: 11/16/2026
* DESCRIPTION		:
*	This file contains the code for the program's entire functionality
*/

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_COUNT 127
#define READ_NAME_BUFFER_SIZE 33

struct NameNode {
	char* name;

	struct NameNode* next;
};

struct NameHashTable {
	struct NameNode* buckets[BUCKET_COUNT];
};

/*
* FUNCTION					: initializeHashTable
* DESCRIPTION				:
*	A function to allocate memory for a hash table and set all pointers to NULL
* PARAMETERS				:
*	None
* RETURNS					:
* 	struct NameHashTable*	: The hash table that was created
*/
struct NameHashTable* initializeHashTable() {
	struct NameHashTable* hashTable = (struct NameHashTable*)malloc(sizeof(struct NameHashTable));
	if (hashTable == NULL) {
		printf("Error: Failed to allocate memory for hash table\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BUCKET_COUNT; i++) {
		hashTable->buckets[i] = NULL;
	}

	return hashTable;
}

/*
* FUNCTION		: generateHash
* DESCRIPTION	:
*	A function to choose a hash which determines the bucket a name will go into
* PARAMETERS	:
*	char* name	: The name to create a hash from
* RETURNS		:
* 	int			: The hashed result
*/
int generateHash(char* name) {
	int hash = 0;

	for (int i = 0; name[i] != '\0'; i++) {
		int ascii = name[i];

		hash = (hash + ascii) % BUCKET_COUNT;
	}

	return hash;
}

/*
* FUNCTION					: createNameNode
* DESCRIPTION				:
*	A function to create a new name node
* PARAMETERS				:
*	char* name				: The name for the node
* RETURNS					:
* 	struct NameNode*		: The node created
*/
struct NameNode* createNameNode(char* name) {
	struct NameNode* node = (struct NameNode*)malloc(sizeof(struct NameNode));
	if (node == NULL) {
		printf("Error: Failed to allocate memory for name node\n");
		exit(EXIT_FAILURE);
	}

	size_t nameLength = strlen(name) + 1; // +1 for null terminator
	node->name = (char*)malloc(nameLength);
	strcpy(node->name, name);

	node->next = NULL;

	return node;
}

/*
* FUNCTION							: insertNameIntoLinkedList
* DESCRIPTION						:
*	A function to insert a new name into a sorted singly linked list
* PARAMETERS						:
*	struct NameNode** head			: A pointer to pointer to the head node of the linked list
*	char* name						: The name to insert
* RETURNS							:
* 	void 	    					: No return value
*/
void insertNameIntoLinkedList(struct NameNode** head, char* name) {
	struct NameNode* node = createNameNode(name);

	if (*head == NULL) {
		*head = node;
	}
	else {
		// insert with sorting

		struct NameNode* current = *head;
		struct NameNode* previous = NULL;

		while (current != NULL && strcmp(name, current->name) > 0) { // names that come after should go to the end, result will be A-Z
			previous = current;
			current = current->next;
		}

		node->next = current;

		if (previous == NULL) { // if this is null then we are inserting at head
			*head = node;
		}
		else {
			previous->next = node;
		}
	}
}

/*
* FUNCTION							: insertNameIntoHashTable
* DESCRIPTION						:
*	A function to insert a new name into the hash table and its linked lists
* PARAMETERS						:
*	struct NameHashTable* hashTable : A pointer to the hash table to insert into
*	char* name						: The name to insert
* RETURNS							:
* 	void 	    					: No return value
*/
void insertNameIntoHashTable(struct NameHashTable* hashTable, char* name) {
	int bucket = generateHash(name);

	insertNameIntoLinkedList(&hashTable->buckets[bucket], name);
}

/*
* FUNCTION		    : removeNewLine
* DESCRIPTION		:
*	Removes the first new line character from a string
* PARAMETERS		:
*	char* str		: The string to remove a new line from
* RETURNS		    :
* 	void 	    	: No return value
*/
void removeNewLine(char* str) {
	str[strcspn(str, "\n")] = '\0';
}

/*
* FUNCTION							: searchLinkedList
* DESCRIPTION						:
*	A function to search a linked list for a specific name
* PARAMETERS						:
*	char* searchName				: The name to search
*	struct NameNode* head			: A pointer to the head of the linked list
*	int* comparisonCount			: A pointer to an integer for storing how many comparisons it took
* RETURNS							:
* 	struct NameNode*				: A pointer to the node containing the name
*/
struct NameNode* searchLinkedList(char* searchName, struct NameNode* head, int* comparisonCount) {
	struct NameNode* result = NULL;

	struct NameNode* current = head;
	while (current != NULL && result == NULL) {
		(*comparisonCount)++;
		if (strcmp(searchName, current->name) == 0) {
			result = current;
		}

		current = current->next;
	}

	return result;
}

/*
* FUNCTION							: searchForNameTwice
* DESCRIPTION						:
*	A function to search a hash table and the linked list of the bucket for a specific name
* PARAMETERS						:
*	char* searchName				: The name to search
*	struct NameNode* head			: A pointer to the head of the linked list
*	int* comparisonCount			: A pointer to an integer for storing how many comparisons it took
* RETURNS							:
* 	struct NameNode*				: A pointer to the node containing the name
*/

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

	// it is placed in the same directory as this source file so that the relative path "names.txt" can be easily used

	FILE* namesFile = fopen("names.txt", "r");
	if (namesFile == NULL) {
		printf("Error: Failed to open names.txt, check that it exists\n");
		return 1;
	}

	struct NameHashTable* hashTable = initializeHashTable();
	struct NameNode* head = NULL;
	
	char nameBuffer[READ_NAME_BUFFER_SIZE];
	while (fgets(nameBuffer, READ_NAME_BUFFER_SIZE, namesFile) != NULL) {
		removeNewLine(nameBuffer);

		insertNameIntoHashTable(hashTable, nameBuffer);
		insertNameIntoLinkedList(&head, nameBuffer);
	}
	
	fclose(namesFile); // we wont touch the file again after inserting so close it now

	if (head == NULL) { // quick check in case a file with nothing was used
		printf("Error: Couldn't find names to insert, check that your names file is not empty\n");
		return 1;
	}

	int linkedListComparisonCount = 0;

	printf("Enter a name to search (Or . to exit):\n");

	// im assuming the user will never search for names longer than 30
	while (1) {
		if (fgets(nameBuffer, READ_NAME_BUFFER_SIZE, stdin) == NULL) {
			printf("Error: Failed to read input from stdin\n");
			return 1;
		}

		removeNewLine(nameBuffer);

		if (strcmp(nameBuffer, ".") == 0) {
			break;
		}

		searchLinkedList(nameBuffer, head, &linkedListComparisonCount);
	}

	printf("\n\n\nSearching the large linked list did %d comparisons\n", linkedListComparisonCount);

	return 0;
}