#include <stdio.h>
#include <stdlib.h>
#include <string.h> //remove later
typedef char byte;

typedef struct Node{
	byte type;
	char character;
	unsigned quantity;
	byte endOfFile;
	
	struct Node *right;
	struct Node *left;

	struct Node *next;

} Node;

typedef struct huffmanCode{

	char character;
	char *charCode;
	unsigned sizeCode;
	byte endOfFile;

	struct huffmanCode *next;

} huffmanCode;



void addNodeLinkedList(Node **head, char character){

	Node *node = malloc(sizeof(Node));
	
	if(*head != NULL){
		node->next = *head;
		*head = node;
	}else{
		*head = node;
		node->next = NULL;
	}

	node->endOfFile = 0;
	node->type = 1;
	node->character = character;
	node->quantity = 1;

}

void addCharacterHuffmanTable(huffmanCode **headTable, char character, char *code, unsigned size){
	
	huffmanCode *node = malloc(sizeof(huffmanCode));

	node->next = *headTable;
	*headTable = node;

	node->character = character;
	node->sizeCode = size;
	node->charCode = malloc(sizeof(char) * size);

	for(unsigned i = 0; i < size; ++i){
		node->charCode[i] = code[i];
	}
	

}

void printLinkedList(Node *head, unsigned size){
	if(head == NULL) return;

	for(unsigned i = 0; i<size; ++i){
		printf("[%c, %i] ", head->character, head->quantity);
		head = head->next;
	}
	printf("\n");
	return;
}

void printHuffmanTable(huffmanCode *headTable){
	while(headTable != NULL){
		if(headTable->character == '\n')
			printf("\\n =");
		else
			printf("%c = ", headTable->character);

		for (unsigned i = 0; i < headTable->sizeCode; ++i){
			
			printf("%c", headTable->charCode[i]);
		}

		printf("\n");

		headTable = headTable->next;
	}
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

huffmanCode *searchCode(huffmanCode *head, unsigned size, char character){
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

unsigned sizeHuffmanTable(huffmanCode *head){
	if(head == NULL) return 0;

	unsigned size = 1;

	while(head->next != NULL){
		++size;
		head = head->next;
	}

	return size;
	
}



void sortLinkedList(Node **head, unsigned size){ /*descending order*/
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





void sortHuffmanTable(huffmanCode **head, unsigned size){ /*descending order*/
	if(head == NULL) return;
	
	huffmanCode *nodeArray[size];
	huffmanCode *nodeAux = *head;

	for(unsigned i = 0; i < size; ++i){
		nodeArray[i] = nodeAux;
		nodeAux = nodeAux->next; 
	}	


		
	int min;
	for(int i = 0; i < size -1; ++i){
		min = i;
		for(int j = i+1; j < size; ++j){
			if(nodeArray[j]->sizeCode < nodeArray[min]->sizeCode)
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




Node *popNodeLinkedList(Node **head){
	if(*head == NULL) return NULL;

	Node *node = *head;
	*head = (*head)->next;

	node->next = NULL;

	return node;

}

void addNodeBinaryTree(Node *left, Node *right, Node **head){
	Node *node = malloc(sizeof(Node));

	node->type = 0;

	node->right = right;
	node->left = left;

	node->quantity = right->quantity + left->quantity;

	node->next = *head;
	*head = node;

}

void buildNodeList(Node **head, char *filename){
	FILE *arq = fopen(filename, "rb");
	int getChar = 0;
	Node *holder = NULL;

	if(arq == NULL){
		printf("Erro ao abrir arquivo\n");
		return;
	}

	while((getChar = fgetc(arq)) != EOF){
		holder = searchNode(*head, sizeLinkedList(*head), (char) getChar);

		if(holder == NULL)
			addNodeLinkedList(head, getChar);
		else
			holder->quantity++;
	}

	addNodeLinkedList(head, '_');
	(*head)->endOfFile = 1;

	fclose(arq);
}


void buildHuffmanTree(Node **head){
	//unsigned size = sizeLinkedList(*head); 

	while(sizeLinkedList(*head) > 1){
		
		sortLinkedList(head, sizeLinkedList(*head));
		addNodeBinaryTree(popNodeLinkedList(head), popNodeLinkedList(head), head);
		
	}
		
	
}

void buildHuffmanTable(Node *head, char *code, char nextCode, unsigned size, huffmanCode **headTable){
	
	if(size == 0){
		buildHuffmanTable(head->left, NULL, '0', size+1, headTable);
		buildHuffmanTable(head->right, NULL, '1', size+1, headTable);
	}else{
		char *newCode = malloc(sizeof(char) * size);
	
		if(size > 1){
				for(unsigned i = 0; i < (size-1); ++i)
					newCode[i] = code[i];				
		}

		newCode[(size-1)] = nextCode;

		if(head->type == 0){
			buildHuffmanTable(head->left, newCode, '0', size+1, headTable);
			buildHuffmanTable(head->right, newCode, '1', size+1, headTable);		
		}else{
			addCharacterHuffmanTable(headTable, head->character, newCode, size);
		}
		
		free(newCode);

	}


}

void getHeightHuffmanTree(Node *head, unsigned *height, unsigned currentLayer){
	
	if(head->type == 0){
		if(head->left != NULL)
			getHeightHuffmanTree(head->left, height, currentLayer + 1);
		if(head->right != NULL)
			getHeightHuffmanTree(head->right, height, currentLayer + 1);
	}else{
		if(currentLayer > *height)
			*height = currentLayer;
	}

}

void freeBinaryTree(Node *head){
	if(head->type == 0){
		freeBinaryTree(head->left);
		freeBinaryTree(head->right);
	}
	free(head);
}


int main(){
	Node *head = NULL;
	
	buildNodeList(&head, "file.txt");
	sortLinkedList(&head, sizeLinkedList(head));
	printLinkedList(head, sizeLinkedList(head));
	buildHuffmanTree(&head);













	huffmanCode *headTable = NULL;

	buildHuffmanTable(head, NULL, ' ', 0, &headTable);
	sortHuffmanTable(&headTable, sizeHuffmanTable(headTable));
	printHuffmanTable(headTable);

	
	freeBinaryTree(head);
	
	/*free head table*/
	unsigned sizeHT = sizeHuffmanTable(headTable);
	huffmanCode *arr[sizeHT];
	huffmanCode *auxHT = headTable;
	for(unsigned i = 0; i < sizeHT; ++i){
		arr[i] = auxHT;
		auxHT = auxHT->next;
	}
	
	for(unsigned i = 0; i < sizeHT; ++i){
		free(arr[i]->charCode);
		free(arr[i]);

	}
	
	/**/
	return 0;
}
