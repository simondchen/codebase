struct Trees{
	Object element;
	Trees *firstChild;
	Trees *nextSibling;
};

struct BinaryTree{
	Object element;	
	BinaryTree *left;
	BinaryTree *right;
};
