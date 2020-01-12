#include <stdio.h>
typedef char byte;

typedef struct Node{
	byte type;
	char character;
	unsigned quantity;
	
	struct Node *right;
	struct Node *left;

} Node;

int main(){
	
	return 0;
}
