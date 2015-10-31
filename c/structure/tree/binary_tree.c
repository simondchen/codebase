#include <stdio.h>
#include <stdlib.h>
typedef int KeyType;
typedef struct Node{
	KeyType key;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
}Node,*PNode;

void insert(PNode *root,KeyType key){
	//initialize the node
	PNode p = (PNode)malloc(sizeof(struct Node));	
	p->key = key;
	p->left=p->right=p->parent=NULL;
	if(*root==NULL){
		*root=p;
		return;
	}
	if((*root)->left==NULL&&key<(*root)->key)
	{
		p->parent=(*root);
		(*root)->left=p;
		return;
	}
	if((*root)->right==NULL&&key>(*root)->key)
	{
		p->parent=(*root);
		(*root)->right=p;
		return;
	}
	if((*root)->key>key){
		insert(&((*root)->left),key);
	}else if((*root)->key<key){
		insert(&((*root)->right),key);
	}else
		return;
}

PNode search(PNode root,KeyType key){
	if(root==NULL)
		return NULL;
	if(root->key==key)
		return root;
	if(root->key<key)
		return search(root->right,key);
	if(root->key>key)
		return search(root->left,key);
}

PNode searchMin(PNode root){
	if(root==NULL)
		return NULL;
	if(root->left!=NULL)
		return searchMin(root->left);
	else
		return root;
}

PNode searchMax(PNode root){
	if(root==NULL)
		return NULL;
	if(root->right!=NULL)
		return searchMax(root->right);
	else
		return root;
}

PNode searchPredecessor(PNode p){
	if(p==NULL)
		return NULL;
	if(p->left!=NULL)
		return searchMax(p->left);
	if(p->parent==NULL)
		return NULL;
	while(p){
		if(p->parent->right==p)
			break;
		p=p->parent;
	}
	return p->parent;
}

PNode searchSuccessor(PNode p){
	if(p==NULL)
		return NULL;
	if(p->right!=NULL)
		return searchMax(p->right);
	if(p->parent==NULL)
		return NULL;
	while(p){
		if(p->parent->left==p)
			break;
		p=p->parent;
	}
	return p->parent;
}

int deleteNode(PNode *root,KeyType key){
	//because you will delete the node , so you must free/release the memory
	PNode p = search((*root),key);
	PNode temp;
	int tempkey;
	if(p==NULL)
		return 0;
	if(p->left==NULL&&p->right==NULL){
		if(p->parent==NULL)
		{
			free(p);
			(*root)=NULL;
		}
		p->parent->left==p?p->parent->left==NULL:p->parent->right==NULL;
		free(p);
	}
	else if(p->left && !(p->right))
	{
		p->left->parent = p->parent;
		if(p->parent==NULL){
			(*root)=p->left;
		}else if(p->parent->left==p)
		{
			p->parent->left=p->left;
		}else{
			p->parent->right=p->left;
		}
		free(p);
	}else if(p->right && !(p->left))
	{
		p->right->parent=p->parent;
		if(p->parent==NULL){
			(*root)=p->right;
		}else if(p->parent->left==p)
		{
			p->parent->left=p->right;
		}else{
			p->parent->right=p->right;
		}
		free(p);
	}else{
		/*temp = searchSuccessor(p);	
		tempkey = temp->key;
		temp->parent=p->parent;
		if(p->parent==NULL){
			(*root)=temp;
		}else if(p->parent->left=p){
			p->parent->left=temp;
		}else{
			p->parent->right=temp;
		}
		deleteNode(root,tempkey);
		free(p);*/ //you don't have to delete the node actually,instead you can replace the value of the node which will simplify the process
		temp = searchSuccessor(p);
		tempkey = temp->key;
		deleteNode(root,tempkey);
		p->key=tempkey;
	}
	return 1;
}

void create(PNode *root,KeyType *keyArray,int length){
	//PNode p = (PNode)malloc(sizeof(struct Node));
	int i;
	for(i=0;i<length;i++)
	{
		insert(root,keyArray[i]);
	}
}

int main(){
	int i;
	PNode root=NULL;
	KeyType nodeArray[11]={15,6,18,3,7,17,20,2,4,13,9};
	create(&root,nodeArray,11);
	printf("%d\n",search(root,3)->left->key);
	PNode pp = search(root,3);
	deleteNode(&root,3);
	printf("%d\n",pp->key);
	return 0;
}













