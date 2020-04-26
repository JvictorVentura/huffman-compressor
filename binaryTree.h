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

void freeBinaryTree(Node *head){
	if(head->type == 0){
		freeBinaryTree(head->left);
		freeBinaryTree(head->right);
	}
	free(head);
}

void buildBinaryTree(Node **head){

	while(sizeLinkedList(*head) > 1){
		
		sortLinkedList(head, sizeLinkedList(*head));
		addNodeBinaryTree(popNodeLinkedList(head), popNodeLinkedList(head), head);
		
	}
		
	
}



unsigned getQuantityOfNodesOfBinaryTree(Node *headTree){
	if(headTree == NULL)
		return 0;
	

	return 1 + getQuantityOfNodesOfBinaryTree(headTree->left) + getQuantityOfNodesOfBinaryTree(headTree->right);
}


void putTreeOnHeap(Node *headTree, Node **heap, int size){
	heap[0] = headTree;

	int index1 = 0;
	int index2 = 1;
	while(index1 < size){
		if(heap[index1]->left != NULL)
			heap[index2++] = heap[index1]->left;

		if(heap[index1]->right != NULL)
			heap[index2++] = heap[index1]->right;
	
		++index1;
	}

}
