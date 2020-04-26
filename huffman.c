#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char byte;
#include "node.h"
#include "huffmanTable.h"
#include "binaryTree.h"
#include "writeCompressedFile.h"


void assignTypeAndChildren(byte *typeAndChildren, Node **heap, int size){

	for(int i = 0; i < size; ++i){

		typeAndChildren[i] = 0;   //clean the variable

	
		if(heap[i]->type == 1)
			typeAndChildren[i] += 4;
		
		if(heap[i]->type == 2)
			typeAndChildren[i] += 8;

		

		if(heap[i]->left != NULL)
			typeAndChildren[i] += 2;

		if(heap[i]->right != NULL)
			typeAndChildren[i] += 1;

	
	}

}



void compress(Node *headTree, huffmanCode *headTable, char *filename){
	if(headTree == NULL || headTable == NULL){
		printf("ERRO\n");
		return;
	}
	
	unsigned size = getQuantityOfNodesOfBinaryTree(headTree);

	Node *aux[size];
	byte *typeAndChildren = malloc(sizeof(byte)*size);

	putTreeOnHeap(headTree, aux, size);
	assignTypeAndChildren(typeAndChildren, aux, size);


	FILE *compFile = fopen("fileCompressed", "wb+");
	
	writeSizeOfHeap(size, compFile);
	writeNodeInformation(typeAndChildren, size, compFile);
	writeNodeCharacter(aux, compFile, size);


	FILE *originalFile = fopen(filename, "rb");

	compressAndWriteFile(originalFile, compFile, headTable);

	fclose(originalFile);
	fclose(compFile);
	free(typeAndChildren);
	
}




int decompress(char *filename){
	FILE *compFile = fopen(filename, "rb");
	if(compFile == NULL){
		printf("Archive does not exist!\n");
		return 1;
	}

	int byteHolder = 0;
	int size = 0;
	for(byte j = 4; j > 0; --j){	//get the size of the arrays (character and type)
		byteHolder =  fgetc(compFile);
		if(byteHolder != EOF){
			size += byteHolder << (j-1)*8;
		}else
			break;
	}


	char *character = malloc(sizeof(byte)*size);		//creating the arrays with the size got from the file
	byte *typeAndChildren = malloc(sizeof(byte)*size);	//



	int index = 0;
	byte x = -4;
	while(index < size){								//getting the information about the characters
		if(x < 0){										//and detach them for each character
			byteHolder = fgetc(compFile);
			x = 4;
		}

		typeAndChildren[index++] = (byteHolder >> x) & 0xF;
		x -= 4;
	}
	

	

	for(int i = 0; i < size; ++i)		//getting characaters from the file
		character[i] = fgetc(compFile);

	


	Node *headTree = malloc(sizeof(Node)*size);			//re-building the tree from the information got from the file
	int index1 = 0;
	int index2 = 1;
	while(index1 < size){
		
		headTree[index1].character = character[index1];

		
		if((typeAndChildren[index1]&4) == 4)
			headTree[index1].type = 1;
		else if((typeAndChildren[index1]&8) == 8)
			headTree[index1].type = 2;
		else{
			headTree[index1].type = 3;

			if((typeAndChildren[index1]&2) == 2)
				headTree[index1].left = &headTree[index2++];
			else
				headTree[index1].left = NULL;

			if((typeAndChildren[index1]&1) == 1)
				headTree[index1].right = &headTree[index2++];
			else
				headTree[index1].right = NULL;

		}

		++index1;
		
	}
	
	//re-building complete


	
	FILE *decompr = fopen("fileDecompressed", "wb+");
	if(decompr == NULL){
		printf("ERRO [1]\n");
		return 1;
	}


	byte endOfFile = 0;
	int bitIndex = 128;
	Node *aux = &headTree[0];
	byteHolder = fgetc(compFile);
	while(endOfFile != 1){			//decompressing
		bitIndex = 128;

		while(bitIndex > 0){
			
			if(aux->type == 1){

				fputc(aux->character, decompr);
				aux = &headTree[0];

			}else if(aux->type == 2){

				endOfFile = 1;
				break;

			}else{
				
				if((byteHolder&bitIndex) == bitIndex){
					aux = aux->right;
					
				}
				else{
					aux = aux->left;
					
				}

				bitIndex = bitIndex >> 1;
			}
		}

		byteHolder = fgetc(compFile);
	}
	//end of decompression
	
	printf("size: %d\n", size);
	//free all memory used and close files

	freeBinaryTree(headTree);

	fclose(decompr);
	fclose(compFile);	
	free(typeAndChildren);
	free(character);
	
	return 0;
}



int main(){

	int option = 0;
	printf("[1]Compress\n[2]Decompress\n[0]Exit\n");
	scanf("%i", &option);
	byte error;

	char *filename = malloc(sizeof(char)*50);

	if(option == 1){
		printf("Write the name of the file to be compressed\n\t[Note: the file name must be less than 50 characters or won't be accepted]\n");
		scanf("%s", filename);

		Node *headTree = NULL;
		
		error = buildNodeList(&headTree, filename);

		if(error == 1)					// if the archive does not exist
			return 0;

		sortLinkedList(&headTree, sizeLinkedList(headTree));
		//printLinkedList(headTree, sizeLinkedList(headTree));
		
		buildBinaryTree(&headTree);

		huffmanCode *headTable = NULL;
		buildHuffmanTable(headTree, NULL, ' ', 0, &headTable);
		sortHuffmanTable(&headTable, sizeHuffmanTable(headTable));
		//printHuffmanTable(headTable);


		//unsigned a = getQuantityOfNodesOfBinaryTree(headTree);
		//printf("%u\n", a);


		compress(headTree, headTable, filename);
		
		

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

	}else if(option == 2){
		printf("Write the name of the file to be decompressed\n\t[Note: the file name must be less than 50 characters or won't be accepted]\n");
		scanf("%s", filename);
		printf("2\n");
		error = decompress(filename);
		if(error == 1)
			return 0;
	}


	return 0;
}
