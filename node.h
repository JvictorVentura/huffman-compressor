typedef struct Node{
	byte type;		//0b01 = char, 0b10 = EOF, 0b11 = Empty Node.
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

void sortLinkedList(Node **head, unsigned size){ 
	if(head == NULL) return;
	
	Node *nodeArray[size];
	Node *nodeAux = *head;

	for(unsigned i = 0; i < size; ++i){
		nodeArray[i] = nodeAux;
		nodeAux = nodeAux->next; 
	}	


		
	int min;
	for(int i = 0; i < size -1; ++i){
		min = i;
		for(int j = i+1; j < size; ++j){
			if(nodeArray[j]->quantity < nodeArray[min]->quantity)
				min = j;
		}
		
		if(min != i){
			nodeAux = nodeArray[i];
			nodeArray[i] = nodeArray[min];
			nodeArray[min] = nodeAux;
		}
		
	}


	for(unsigned i=0; i < (size - 1); ++i){
		nodeArray[i]->next = nodeArray[i+1];
	}
	
	nodeArray[size-1]->next = NULL;
		
	*head = nodeArray[0];

}


int buildNodeList(Node **head, char *filename){
	FILE *arq = fopen(filename, "rb");

	if(arq == NULL){
		printf("Erro ao abrir arquivo\n");
		return 1;
	}

	
	int getChar = 0;
	Node *holder = NULL;
	
	while((getChar = fgetc(arq)) != EOF){
		holder = searchNode(*head, sizeLinkedList(*head), (char) getChar);

		if(holder == NULL)
			addNodeLinkedList(head, getChar);
		else
			holder->quantity++;
	}

	addNodeLinkedList(head, '_'); //codigo de parada (EOF)
	(*head)->type = 2;			  //

	fclose(arq);

	return 0;
}

Node *popNodeLinkedList(Node **head){
	if(*head == NULL) return NULL;

	Node *node = *head;
	*head = (*head)->next;

	node->next = NULL;

	return node;

}




/*
void printLinkedList(Node *head, unsigned size){
	if(head == NULL) return;

	for(unsigned i = 0; i<size; ++i){
		printf("[%c, %i] ", head->character, head->quantity);
		head = head->next;
	}
	printf("\n");
	return;
}*/