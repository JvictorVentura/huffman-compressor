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


//	Run through the heap and store the information of each node on the array
//	nodeInformation. Each node uses half of a byte to store its information.
void assignNodeInformation(byte *nodeInformation, Node **heap, int size){

	for(int i = 0; i < size; ++i){
		nodeInformation[i] = 0;   //clean the array 

	
		if(heap[i]->type == 1)			//
			nodeInformation[i] += 4;	//
										//store the type of the node
		if(heap[i]->type == 2)			//
			nodeInformation[i] += 8;	//

		

		if(heap[i]->left != NULL)		// if the node has children on the left, turn this bit on
			nodeInformation[i] += 2;

		if(heap[i]->right != NULL)
			nodeInformation[i] += 1;	// if the node has children on the right, turn this bit on

	
	}

}


//	Based on the given filename, creates a new with the extension .huff .
//	if the filename already has an extension, or multiples extensions
//	the function replaces the last.
char *newFileName(char *fileName, byte *dotLocation, byte *sizeOfExtension){
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

void writeOriginalExtensionOnFile(FILE *compressedFile, char *fileName, int sizeOfExtension, int  extensionLocation){

	fputc(sizeOfExtension, compressedFile);
	if(sizeOfExtension > 0)
		for(int i = 0; i < sizeOfExtension; ++i)
			fputc(fileName[extensionLocation + i], compressedFile);
		
	
}

void writeMagicNumber(FILE *compressedFile){
	char magicNumber[4] = {'h', 'u', 'f', 'f'};
	for(int i = 0; i < 4; ++i)
		fputc(magicNumber[i], compressedFile);
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
	byte *nodeInformation = malloc(sizeof(byte)*size);
	printf("size = %d\n", size);
	putTreeOnHeap(headTree, aux, size);
	assignNodeInformation(nodeInformation, aux, size);

	FILE *compressedFile = fopen(nFileName, "wb+");

	writeMagicNumber(compressedFile);
	writeOriginalExtensionOnFile(compressedFile, filename, sizeOfExtension, dotLocation+1);
	writeSizeOfHeap(size, compressedFile);
	writeNodeInformation(nodeInformation, size, compressedFile);
	writeNodeCharacter(aux, compressedFile, size);


	FILE *originalFile = fopen(filename, "rb");

	compressAndWriteFile(originalFile, compressedFile, headTable);

	fclose(originalFile);
	fclose(compressedFile);
	free(nodeInformation);
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

int checkMagicNumber(FILE *compressedFile){
	char magicNumber[4] = {'h','u','f','f'};
	for(int i = 0; i < 4; ++i)
		if(((char) fgetc(compressedFile)) != magicNumber[i]){
			printf("Arquivo não é do formato .huff\n");
			return 1;
		}
	return 0;
}

int decompress(char *filename){
	FILE *compFile = fopen(filename, "rb");
	if(compFile == NULL){
		printf("Archive does not exist!\n");
		return 1;
	}
	if(checkMagicNumber(compFile))
		return 1;
			
	char *orgFileName = originalFileName(compFile, filename);

	int size = getSizeOfHeap(compFile);
	printf("size = %d\n", size);
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
