#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "printBinary.h"
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

char *newFileName(char *fileName, byte *dotLocation, byte *sizeOfExtension){
	printf("%c\n", fileName[0]);
	int sizeOfFileName = strlen(fileName);
	*dotLocation = sizeOfFileName - 1;

	for(int i = sizeOfFileName - 1; i >= 0; --i){
		if(fileName[i] == '.'){
			*dotLocation = i;
			break;
		}else
			++(*sizeOfExtension);
	}

	if(sizeOfFileName == *sizeOfExtension)
		*sizeOfExtension = 0;

	char *nFileName = NULL;

	if(*dotLocation != sizeOfFileName - 1){
		nFileName = malloc((*dotLocation) + 6);
		
		for(int i = 0; i < sizeOfFileName; ++i)
			nFileName[i] = fileName[i];

		nFileName[(*dotLocation) + 1] = 'h';
		nFileName[(*dotLocation) + 2] = 'u';
		nFileName[(*dotLocation) + 3] = 'f';
		nFileName[(*dotLocation) + 4] = 'f';
		nFileName[(*dotLocation) + 5] = '\0';
	}else{
		nFileName = malloc(sizeOfFileName + 5);

		for(int i = 0; i < sizeOfFileName; ++i)
			nFileName[i] = fileName[i];

		nFileName[sizeOfFileName] = '.';
		nFileName[sizeOfFileName + 1] = 'h';
		nFileName[sizeOfFileName + 2] = 'u';
		nFileName[sizeOfFileName + 3] = 'f';
		nFileName[sizeOfFileName + 4] = 'f';
		nFileName[sizeOfFileName + 5] = '\0';

	}

	return nFileName;
}

void writeExtensionOnFile(FILE *compressedFile, char *fileName, int sizeOfExtension, int  extensionLocation){

	fputc(sizeOfExtension, compressedFile);
	printf("%d\n", sizeOfExtension);
	if(sizeOfExtension > 0)
		for(int i = 0; i < sizeOfExtension; ++i)
			fputc(fileName[extensionLocation + i], compressedFile);
		
	
}



void compress(Node *headTree, huffmanCode *headTable, char *filename){
	if(headTree == NULL || headTable == NULL){
		printf("ERRO\n");
		return;
	}

	byte dotLocation = 0;
	byte sizeOfExtension = 0;

	char *nFileName = newFileName(filename, &dotLocation, &sizeOfExtension);

	unsigned size = getQuantityOfNodesOfBinaryTree(headTree);
	Node *aux[size];
	byte *typeAndChildren = malloc(sizeof(byte)*size);

	putTreeOnHeap(headTree, aux, size);
	assignTypeAndChildren(typeAndChildren, aux, size);


	FILE *compFile = fopen(nFileName, "wb+");
	fputc('h', compFile);
	fputc('u', compFile);
	fputc('f', compFile);
	fputc('f', compFile);

	writeExtensionOnFile(compFile, filename, sizeOfExtension, dotLocation+1);
	writeSizeOfHeap(size, compFile);
	writeNodeInformation(typeAndChildren, size, compFile);
	writeNodeCharacter(aux, compFile, size);


	FILE *originalFile = fopen(filename, "rb");

	compressAndWriteFile(originalFile, compFile, headTable);

	fclose(originalFile);
	fclose(compFile);
	free(typeAndChildren);
	free(nFileName);

	
}


char *originalFileName(FILE *compressedFile, char *fileName){

	int sizeOfExtension = fgetc(compressedFile);
	int sizeOfFileName = strlen(fileName);
	if(sizeOfExtension == 0){
		char *originalName = malloc(sizeOfFileName - 4);
		int i;

		for(i = 0; i < sizeOfFileName - 5; ++i)
			originalName[i] = fileName[i];

		originalName[i] = '\0';
		return originalName;
	}else{
		char extension[sizeOfExtension];

		for(int i = 0; i < sizeOfExtension; ++i)
			extension[i] = (char) fgetc(compressedFile);

		char *originalName = malloc((sizeOfFileName - 3 + sizeOfExtension));

		int i;
		for(i = 0; i < sizeOfFileName - 4; ++i)
			originalName[i] = fileName[i];
		int j;
		for(j = 0; j < sizeOfExtension; ++j)
			originalName[i + j] = extension[j];
		originalName[i + j] = '\0';

		return originalName;
	}
}


int decompress(char *filename){
	FILE *compFile = fopen(filename, "rb");
	if(compFile == NULL){
		printf("Archive does not exist!\n");
		return 1;
	}

	char magicNumber[4] = {'h','u','f','f'};
	for(int i = 0; i < 4; ++i)
		if(((char) fgetc(compFile)) != magicNumber[i]){
			printf("Arquivo não é do formato .huff\n");
			return 1;
		}
			
	char *orgFileName = originalFileName(compFile, filename);

	int size = getSizeOfHeap(compFile);

	char *character = malloc(sizeof(byte)*size);		
	byte *typeAndChildren = malloc(sizeof(byte)*size);	


	getNodeInformation(typeAndChildren, size, compFile);
	getCharacter( character, compFile, size);


	Node *headTree = malloc(sizeof(Node)*size);		

	reconstructBinaryTree(headTree, typeAndChildren, character, size);
	
	
	FILE *decompr = fopen(orgFileName, "wb+");
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
	free(orgFileName);
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
