#include <stdio.h>
#include <stdlib.h>
//#include <string.h> //remove later
#include "printBinary.h"
typedef char byte;

typedef struct Node{
	byte type;		//1char 2EOF 3nó vazio
	char character;
	unsigned quantity;
	
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

	node->type = 1;
	node->character = character;
	node->quantity = 1;

}

void addCharacterHuffmanTable(huffmanCode **headTable, char character, char *code, unsigned size, byte typeChar){
	
	huffmanCode *node = malloc(sizeof(huffmanCode));

	node->next = *headTable;
	*headTable = node;

	node->character = character;
	node->sizeCode = size;
	node->endOfFile = typeChar;
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

		printf("   %i\n", headTable->endOfFile);

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

huffmanCode *searchCode(huffmanCode *head, unsigned size, char character, byte typeChar){
	if(head == NULL) return NULL;
	
	for(unsigned i = 0; i<size ; ++i){
		if((head->character == character) && (head->endOfFile == typeChar))
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
	node->character = '-';

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

	addNodeLinkedList(head, '_'); //codigo de parada
	(*head)->type = 2;			  //

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
			if(head->type == 2)			// IF eof
				addCharacterHuffmanTable(headTable, head->character, newCode, size, 1);
			else
				addCharacterHuffmanTable(headTable, head->character, newCode, size, 0);
		}
		
		free(newCode);

	}


}


void freeBinaryTree(Node *head){
	if(head->type == 0){
		freeBinaryTree(head->left);
		freeBinaryTree(head->right);
	}
	free(head);
}


unsigned getQuantityOfNodes(Node *headTree){
	if(headTree == NULL){
		return 0;
	}

	return 1 + getQuantityOfNodes(headTree->left) + getQuantityOfNodes(headTree->right);
}


void compress(Node *headTree, huffmanCode *headTable, char *filename){
	if(headTree == NULL || headTable == NULL){
		printf("ERRO\n");
		return;
	}
	
	unsigned size = getQuantityOfNodes(headTree);

	//printf("%u\n", size);
	
	Node *aux[size];
	byte *typeAndChildren = malloc(sizeof(byte)*size);

	aux[0] = headTree;
	unsigned quantity = 1;

	//zera o array
	for(unsigned i = 0; i < size; ++i)
		typeAndChildren[i] = 0;	



	for(unsigned i=0; quantity < size; ++i){
		if(aux[i]->left != NULL){
			aux[quantity++] = aux[i]->left;
			typeAndChildren[i] += 2;
		}

		if(aux[i]->right != NULL){
			aux[quantity++] = aux[i]->right;
			typeAndChildren[i] += 1;
		}
		


	}



	for(unsigned i = 0; i < size; ++i){
		if(aux[i]->type == 2)
			typeAndChildren[i] += 8;
		if(aux[i]->type == 1)
			typeAndChildren[i] += 4;
	}



	FILE *compFile = fopen("fileCompressed", "wb+");

	for(byte i = 4; i > 0;--i)
		fputc(size >> (i-1)*8, compFile);

	for(unsigned i = 0; i < size; ++i)
		fputc(typeAndChildren[i], compFile);

	for(unsigned i = 0; i < size; ++i)
		fputc(aux[i]->character, compFile);



	FILE *orgFile = fopen(filename, "rb");

	huffmanCode *code = NULL;
	unsigned codeIndex = 0;
	byte endOfFile = 0;
	int charHolder = 0; 
	unsigned byteCode = 0;
	int byteIndex = 128;

	while(endOfFile != 1){
		byteIndex = 128;
		while(byteIndex > 0){
			//printBinary(byteCode);
			if(code == NULL){
				if((charHolder = fgetc(orgFile)) == EOF){
					endOfFile = 2;
					code = searchCode(headTable, sizeHuffmanTable(headTable), '_',1);
					printf("%c\n", code->character);
				}else{
					code = searchCode(headTable, sizeHuffmanTable(headTable), charHolder,0);

					//printf("%c\n", code->character);
					/*printf("charHolder\n");
					printf("%c ", charHolder);*/
					//printBinary(charHolder);
					//printf("\n");
				}
				
				codeIndex = 0;

			}

			//printf("codeIndex %i\n", codeIndex);
			//printf("code->sizeCode %i\n", code->sizeCode);


			if(codeIndex >= code->sizeCode){
				if(endOfFile == 2){
					endOfFile = 1;
					break;
				}
				code = NULL;
				codeIndex = 0;
			}else{
				if((code->charCode[codeIndex++]) == '1'){
					byteCode += byteIndex;;
				}
				byteIndex = byteIndex >> 1;
			}


		}
		
		//printBinary(byteCode);

		fputc(byteCode, compFile);
		byteCode = 0;
	}
	fclose(orgFile);
	fclose(compFile);
	free(typeAndChildren);
	
}









void decompress(char *filename){
	FILE *compFile = fopen(filename, "rb");
	int byteHolder = 0;
	int size = 0;
	for(byte j = 4; j > 0; --j){
		byteHolder =  fgetc(compFile);
		if(byteHolder != EOF){
			size += byteHolder << (j-1)*8;
		}else
			break;
	}

	char *character = malloc(sizeof(byte)*size);
	byte *typeAndChildren = malloc(sizeof(byte)*size);

	
	for(int i = 0; i < size; ++i)
		typeAndChildren[i] = fgetc(compFile);
		
	
	for(int i = 0; i < size; ++i)
		character[i] = fgetc(compFile);

	//printf("%d\n", size);

	/*for(int i = 0; i < size; ++i)
		printf("[%c]", character[i]);
*/
	Node *headTree = malloc(sizeof(Node)*size);
	int p1 = 0;
	int p2 = 1;

	while(p1 < size){
		
		headTree[p1].character = character[p1];

		
		if((typeAndChildren[p1]&4) == 4)
			headTree[p1].type = 1;
		else if((typeAndChildren[p1]&8) == 8)
			headTree[p1].type = 2;
		else{
			headTree[p1].type = 3;

			if((typeAndChildren[p1]&2) == 2)
				headTree[p1].left = &headTree[p2++];
			else
				headTree[p1].left = NULL;

			if((typeAndChildren[p1]&1) == 1)
				headTree[p1].right = &headTree[p2++];
			else
				headTree[p1].right = NULL;

		}

		++p1;
		
	}
	printf("\n\n");

	/*for(int i = 0; i < size; ++i)
		printf("[%c%d]", headTree[i].character, headTree[i].type);
*/
	

	
	FILE *decompr = fopen("fileDecompressed", "wb+");
	if(decompr == NULL){
		printf("ERRO [1]\n");
		return;
	}


	/*for(int i = 0; i < size; ++i)
		if(headTree[i].character == character[i])
			printf("IGUAL\n");
		else
			printf("DIFERENTE\n");
*/


	byte endOfFile = 0;
	int bitIndex = 128;
	Node *aux = &headTree[0];
	byteHolder = fgetc(compFile);

	while(endOfFile != 1){
		bitIndex = 128;

		while(bitIndex > 0){
			
			if(aux->type == 1){
				//printf("%c", aux->character);
				fputc(aux->character, decompr);
				aux = &headTree[0];
			}else if(aux->type == 2){
				endOfFile = 1;
				break;
				//printf("ENTROU CARALHO\n");
			}else{
				//printf("%c ", aux->character);
				//printf("%d\n", aux->type);
				if((byteHolder&bitIndex) == bitIndex){
					aux = aux->right;
					//printf("right\n");
				}
				else{
					aux = aux->left;
					//printf("left\n");
				}

				bitIndex = bitIndex >> 1;
			}
		}

		byteHolder = fgetc(compFile);
	}
	
	
	fclose(decompr);	
	free(typeAndChildren);
	fclose(compFile);
}



int main(){
	
	Node *headTree = NULL;
	
	buildNodeList(&headTree, "file");
	sortLinkedList(&headTree, sizeLinkedList(headTree));
	printLinkedList(headTree, sizeLinkedList(headTree));
	
	buildHuffmanTree(&headTree);

	huffmanCode *headTable = NULL;
	buildHuffmanTable(headTree, NULL, ' ', 0, &headTable);
	sortHuffmanTable(&headTable, sizeHuffmanTable(headTable));
	//printHuffmanTable(headTable);

	compress(headTree, headTable, "file");
	
	//free binaryTree

	freeBinaryTree(headTree);
	
	//free headTable
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
	
	
	//decompress("fileCompressed");


	return 0;
}
