#include <stdio.h>
#include <stdlib.h>
typedef char byte;

typedef struct Node{
	byte type;
	char character;
	unsigned quantity;
	
	struct Node *right;
	struct Node *left;

	struct Node *next;

} Node;


void addNodeLinkedList(Node **head, char character){

	Node *node = malloc(sizeof(Node));
	
	if(*head != NULL){
		node->next = *head;
		*head = node;
	}else{
		*head = node;
		node->next = NULL;
	}

	node->type = 1;
	node->character = character;
	node->quantity = 1;

}

int printLinkedList(Node *head, unsigned size){
	if(head == NULL) return 1;

	for(unsigned i = 0; i<size; ++i){
		printf("[%c, %i] ", head->character, head->quantity);
		head = head->next;
	}
	printf("\n");
	return 0;
}

Node *searchNode(Node *head, unsigned size, char character){
	if(head == NULL) return NULL;
	
	for(unsigned i = 0; i<size ; ++i){
		if(head->character == character)
			return head;
		
		
		head = head->next;
	
	}
	
	
	return NULL;
}

unsigned sizeLinkedList(Node *head){
	if(head == NULL) return 0;

	unsigned size = 1;

	while(head->next != NULL){
		++size;
		head = head->next;
	}

	return size;

}


int main(){
	Node *head;
	
	addNodeLinkedList(&head, 'a');
	printLinkedList(head, sizeLinkedList(head));

	addNodeLinkedList(&head, 'b');
	printLinkedList(head, sizeLinkedList(head));

	addNodeLinkedList(&head, 'c');
	printLinkedList(head, sizeLinkedList(head));

	
	Node *test;
	test = searchNode(head, sizeLinkedList(head), 'a');
	test->quantity += 5;
	test = searchNode(head, sizeLinkedList(head), 'c');
	test->quantity += 3;

	printLinkedList(head, sizeLinkedList(head));



	//backup to free memory later
	
	
		

	//freeing memory
	free(head->next->next);
	free(head->next);
	free(head);

	return 0;
}
