//	The size of the file is saved in 4 uint8_ts,
//	I dont know if I am overestimating the size of
//	the heap, but will stay this way for now.
int getSizeOfHeap( FILE *compressedFile){

	int size = 0;
	int uint8_tGot = 0;

	for( int8_t bitShift = 3; bitShift >= 0; --bitShift){
		uint8_tGot = fgetc(compressedFile);
		size += uint8_tGot << (bitShift*8);
	}
	
	return size;
}

//	As was said on the other function, the information of
//	2 Nodes is stored in one byte, so this function detach
//	the information.
void getNodeInformation( uint8_t *nodeInformation, int size, FILE *compressedFile){

	int byteHOlder = 0;
	int8_t bitShift = -4;
	int index = 0;

	while( index < size){
		if(bitShift < 0){
			byteHOlder = fgetc(compressedFile);
			bitShift = 4;
		}

		nodeInformation[index++] = (uint16_t) (byteHOlder >> bitShift);
		bitShift -= 4;	
	
	}

}

//	Get the Node characters for the heap.
void getCharacter(char *character, FILE *compressedFile, int size){

	for(int i = 0; i < size; ++i)
		character[i] = fgetc(compressedFile);

}

void reconstructBinaryTree(Node *headTree, uint8_t *nodeInformation, char *character, int size){

	int index1 = 0;
	int index2 = 1;

	while(index1 < size){
	
		headTree[index1].character = character[index1];

		if((nodeInformation[index1] & 0x4) == 0x4)
			headTree[index1].type = 0x1;
		else if((nodeInformation[index1] & 0x8) == 0x8)
			headTree[index1].type = 0x2;
		else{
			headTree[index1].type = 0x3;

			if((nodeInformation[index1] & 0x2) == 0x2)
				headTree[index1].left = &headTree[index2++];
			else
				headTree[index1].left = NULL;


			if((nodeInformation[index1] & 0x1) == 0x1)
				headTree[index1].right = &headTree[index2++];
			else
				headTree[index1].right = NULL;

			}
		++index1;

	}

}

void decompressAndWrite( FILE *compressedFile, FILE *decompressedFile, Node *headTree){
	typedef char byte;
	byte endOfFile = 0;
	int bitIndicator = 0x80;
	Node *aux = &headTree[0];
	int byteHolder = fgetc(compressedFile);

	while(endOfFile != 1){

		bitIndicator = 0x80;

		while(bitIndicator > 0){
			if(aux->type == 1){
				fputc(aux->character, decompressedFile);
				aux = &headTree[0];
			}else if(aux->type == 2){
				endOfFile = 1;
				break;
			}else{
				if((byteHolder & bitIndicator) == bitIndicator)
					aux = aux->right;
				else
					aux = aux->left;

				bitIndicator >>= 1;
			}
		}
		
		byteHolder = fgetc(compressedFile);
	}

}
