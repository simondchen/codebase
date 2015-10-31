#include <stdio.h>
#include <iostream>
using namespace std;

struct ListNode{
	int value;
	struct ListNode *next;
	ListNode(int x) : value(x),next(NULL) {}
};

		struct ListNode *addcarry(struct ListNode *l){
			struct ListNode *tmp=l;
			if(l==NULL){
				return new ListNode(1);
			}
			do{
				int v = l->value;
				l->value+=1;
				//carry=(l->value>=10)?1:0;
				if((v+1)<10)
					return tmp;
				l=l->next;
			}while(l->next);
			return tmp;
		}
		struct ListNode *addTwoNumbers(struct ListNode *l1,struct ListNode *l2){
			int carry = 0;
			struct ListNode *result = new NodeList(0);
			struct ListNode *tmp = result;
			do{
				int v1 = l1->value;		
				int v2 = l2->value;
				 = (v1+v2)%10+carry;
				if((v1+v2)>=10)
					carry=1;
				else
					carry=0;
				l1=l1->next;
				l2=l2->next;
			}while(l1 && l2);
			if(carry==0){
				tmp=l1?l1:l2;
			}else{
				tmp=l1?addcarry(l1):addcarry(l2);
			}
			return result;
		}

int main(){
	struct ListNode	*l1 = new ListNode(2);
	l1->next = new ListNode(4);
	l1->next->next = new ListNode(3);
	struct ListNode *l2 = new ListNode(5);
	l2->next = new ListNode(6);
	l2->next->next = new ListNode(4);
	struct ListNode *ret = addTwoNumbers(l1,l2);
	do{
		printf("%d\n",ret->value);
		ret=ret->next;
	}while(ret);
	return 0;
}






