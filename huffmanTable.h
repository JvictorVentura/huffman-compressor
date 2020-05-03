typedef struct huffmanCode{

	char character;
	char *charCode;
	unsigned sizeCode;
	byte endOfFile;

	struct huffmanCode *next;

} huffmanCode;



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

/*
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
}*/

huffmanCode *searchCode(huffmanCode *head, unsigned size, char character, byte typeChar){
	if(head == NULL) return NULL;
	
	for(unsigned i = 0; i<size ; ++i){
		if((head->character == character) && (head->endOfFile == typeChar))
			return head;
		
		
		head = head->next;
	
	}
	
	return NULL;
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

//	Sort the huffman table so the search doesnt take too long.
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
