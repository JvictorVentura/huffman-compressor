void writeSizeOfHeap(uint16_t size,  FILE *compressedFile){
	for(uint8_t i = 4 ; i > 0; --i)
		//fputc(size >> ((i - 1) * 8), compressedFile);
		fputc(size >> ((i - 1) * 8), compressedFile);
}

//	Writes the information of the Nodes on 4 bits
//	saving space by fitting the information of 2 Nodes
//	on one byte.
void writeNodeInformation(uint8_t *nodeInformation, uint16_t size, FILE *compressedFile){

	uint8_t biteWritten = 0;
	int index = 0;
	int8_t bitShift = 4;

	while(index < size){

		if(bitShift < 0){
			fputc(biteWritten, compressedFile);
			bitShift = 4;
			biteWritten = 0;
		}
		
		biteWritten += nodeInformation[index++] << bitShift;
		bitShift -= 4;

	}

	fputc(biteWritten, compressedFile);

}

void writeNodeCharacter(Node **node, FILE *compressedFile, uint16_t size){

	for(uint16_t i = 0; i < size; ++i)
		fputc(node[i]->character, compressedFile);

}

//	Get the character/byte and wite its code on the compressed file.
//	Refactor this function would be great.
void compressAndWriteFile(FILE *originalFile, FILE *compressedFile, huffmanCode *headTable){

	huffmanCode *code = NULL;
	uint8_t endFlag = 0;
	int bitIndicator = 0x80;
	uint16_t biteWritten = 0;
	int character = 0;
	uint16_t codeStringIndex = 0;

	while(endFlag != 1){

		while(bitIndicator > 0){
			if(code == NULL){
				if( (character = fgetc(originalFile)) ==  EOF){
					endFlag = 2;
					code = searchCode(headTable, sizeHuffmanTable(headTable), '_', 1);
				}else
					code = searchCode(headTable, sizeHuffmanTable(headTable), character, 0);
			}

			if(codeStringIndex >= code->sizeCode){
				if(endFlag == 2){
					endFlag = 1;
					break;
				}
				code = NULL;
				codeStringIndex = 0;
			}else{
				if( code->charCode[codeStringIndex++] == '1')
					biteWritten += bitIndicator;
				bitIndicator =  bitIndicator >> 1;
			}

		}

		
		fputc(biteWritten, compressedFile);
		biteWritten = 0;
		bitIndicator = 0x80;
	
	}


}
