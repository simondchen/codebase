#include <stdio.h>
#include <stdlib.h>
#define LH -1
#define EH 0
#define RH 1

typedef struct Node{
	int key;
	int bFactor;
	struct Node *parent;
	struct Node *lchild;
	struct Node *rchild;
}Node,*PNode;

int isTaller=0;
PNode root=NULL;

void rRotate(PNode node){
	//yes we should update all parent info,but we may not need the parent info except the first one in preorder! if we use parent info,then we must        //update it!!! 
	PNode lc=node->lchild;
	node->lchild=lc->rchild;
	//lc->rchild may be null
	if(lc->rchild)
		lc->rchild->parent=node;
	lc->rchild=node;
	lc->parent=node->parent;
	if(!(node->parent)){
		root=lc;		
	}else if(node->parent->lchild->key==node->key)
	{
		node->parent->lchild=lc;
	}
	else
	{
		node->parent->rchild=lc;
	}
	node->parent=lc;
}

void lRotate(PNode node){
	PNode rc=node->rchild;
	node->rchild=rc->lchild;
	if(rc->lchild)
		rc->lchild->parent=node;
	rc->lchild=node;
	rc->parent=node->parent;
	if(!(node->parent))
	{
		root=rc;
	}else if(node->parent->lchild->key==node->key)
	{
		node->parent->lchild=rc;
	}else
	{
		node->parent->rchild=rc;
	}
	node->parent=rc;
}

void leftBalance(PNode node){
	PNode lc=node->lchild;
	switch(lc->bFactor){
		case LH:{
			node->bFactor=lc->bFactor=EH;
			rRotate(node);
			break;
		}
		case RH:{
			PNode ld=lc->rchild;
			switch(ld->bFactor){
				case LH:{
					node->bFactor=RH;
					lc->bFactor=EH;
					break;
				}
				case EH:{
					node->bFactor=lc->bFactor=EH;
					printf("don't think will come here\n");
					break;
				}
				case RH:{
					node->bFactor=EH;
					lc->bFactor=LH;
					break;
				}
			}
			ld->bFactor=EH;
			lRotate(node->lchild);
			rRotate(node);
			break;
		}
	}
}

void rightBalance(PNode node){
	PNode rc=node->rchild;
	switch(rc->bFactor){
		case LH:{
			PNode rd=rc->lchild;
			switch(rd->bFactor){
				case LH:{
					node->bFactor=EH;
					rc->bFactor=RH;
					break;
				}
				case EH:{
					node->bFactor=rc->bFactor=EH;
					printf("don't think will come here\n");
					break;
				}
				case RH:{
					node->bFactor=LH;
					rc->bFactor=EH;
					break;
				}
			}
			rd->bFactor=EH;
			rRotate(node->rchild);
			lRotate(node);
			break;
		}
		case RH:{
			node->bFactor=rc->bFactor=EH;
			lRotate(node);
			break;
		}
	}
}

int insert(int key){
	if(root==NULL)
	{
		PNode p=(PNode)malloc(sizeof(struct Node));
		p->key=key;
		root=p;
		return 0;
	}else{
		return insertAvl(root,key);
	}
}
int insertAvl(PNode node,int key){
	if((node->key)==key)
	{
		printf("key already exist\n");
		return -1;
	}else if((node->key)>key)
	{
		if((node->lchild)==NULL)
		{
			PNode p=(PNode)malloc(sizeof(struct Node));
			p->key=key;
			node->lchild=p;
			p->parent=node;
			isTaller=1;
		}
		else{
			insertAvl(node->lchild,key);
		}
		if(isTaller==1){
			switch(node->bFactor){
				case LH:{
					leftBalance(node);	
					isTaller=0;
					break;
				}
				case EH:{
					node->bFactor=LH;
					isTaller=1;
					break;
				}
				case RH:{
					node->bFactor=EH;
					isTaller=0;	
					break;
				}
			}
		}
	}else{
		if((node->rchild)==NULL)
		{
			PNode p=(PNode)malloc(sizeof(struct Node));
			p->key=key;
			node->rchild=p;
			p->parent=node;
			isTaller=1;
		}
		else{
			insertAvl(node->rchild,key);
		}
		if(isTaller==1){
			switch(node->bFactor){
				case LH:{
					node->bFactor=EH;
					isTaller=0;
					break;
				}
				case EH:{
					node->bFactor=RH;
					isTaller=1;
					break;
				}
				case RH:{
					rightBalance(node);
					isTaller=0;
					break;
				}
			}
		}
	}
	return 0;
}


int preorderTraverse(PNode p){
	if(p!=NULL)
	{
		printf("%d\n",p->key);
		preorderTraverse(p->lchild);
		preorderTraverse(p->rchild);
	}
	return 0;
}

int main(){
	insert(80);
	insert(60);
	insert(90);
	insert(85);
	insert(120);
	insert(100);
	preorderTraverse(root);
	return 0;
}
