#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printBinary.h"
typedef char byte;
#include "node.h"
#include "huffmanTable.h"
#include "binaryTree.h"
#include "writeCompressedFile.h"
#include "decompressFile.h"

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
	

	int size = getSizeOfHeap(compFile);

	char *character = malloc(sizeof(byte)*size);		
	byte *typeAndChildren = malloc(sizeof(byte)*size);	


	getNodeInformation(typeAndChildren, size, compFile);
	getCharacter( character, compFile, size);


	Node *headTree = malloc(sizeof(Node)*size);		

	reconstructBinaryTree(headTree, typeAndChildren, character, size);
	
	
	FILE *decompr = fopen("fileDecompressed", "wb+");
	if(decompr == NULL){
		printf("ERRO [1]\n");
		return 1;
	}

	decompressAndWrite(compFile, decompr, headTree);
		
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
		error = decompress(filename);
		if(error == 1)
			return 0;
	}


	return 0;
}
