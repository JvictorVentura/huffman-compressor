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
	}

	node->type = 1;
	node->character = character;
	node->quantity = 1;

}

int printLinkedList(Node *head){
	if(head == NULL) return 1;


	while(head->next != NULL){
		printf("[%c, %i] ", head->character, head->quantity);
		head = head->next;
	}
	
	return 0;
}



int main(){
	Node *head;
	addNodeLinkedList(&head, 'a');
	printLinkedList(head);
	free(head);	

	return 0;
}
