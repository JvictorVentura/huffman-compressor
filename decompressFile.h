int getSizeOfHeap( FILE *compressedFile){

	int size = 0;
	int byteGot = 0;

	for( byte bitShift = 3; bitShift >= 0; --bitShift){
		byteGot = fgetc(compressedFile);
		//printf("%x\n", byteGot);
		size += byteGot;
	}
	
	return size;
}

void getNodeInformation( byte *nodeInformation, int size, FILE *compressedFile){

	int byteHolder = 0;
	byte bitShift = -4;
	int index = 0;

	while( index < size){
		if(bitShift < 0){
			byteHolder = fgetc(compressedFile);
			bitShift = 4;
		}

		nodeInformation[index++] = (unsigned) (byteHolder >> bitShift);
		bitShift -= 4;	
	
	}

}

void getCharacter(char *character, FILE *compressedFile, int size){

	for(int i = 0; i < size; ++i)
		character[i] = fgetc(compressedFile);

}

void reconstructBinaryTree(Node *headTree, byte *nodeInformation, char *character, int size){

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
