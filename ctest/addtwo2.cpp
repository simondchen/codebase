#include <iostream>
using namespace std;
struct ListNode{
	int val;
	ListNode *next;
	ListNode(int val) : val(val),next(NULL){}
};

class Solution{
	public:	
		ListNode *addcarry(ListNode *l){
			ListNode *tmp=l;	
			while(true){
				int v = l->val;
				l->val = (v+1)%10;
				if((1+v)<10)
					return tmp;
				if(l->next==NULL)
					break;
				l=l->next;
			}
			l->next = new ListNode(1);
			return tmp;
		}
		ListNode *addTwoNumbers(ListNode *l1,ListNode *l2){
			int carry=0;	
			ListNode *tmp=l1;
			while(true){
				int v1 = l1->val;
				int v2 = l2->val;
				l1->val=(v1+v2+carry)%10;
				if((v1+v2+carry)>=10)
					carry=1;
				else
					carry=0;
				if(l1->next==NULL || l2->next==NULL)
					break;
				l1=l1->next;
				l2=l2->next;
			}
			if(carry==0){
				l1->next=l1->next?l1->next:l2->next;
			}else{
				if(l1->next==NULL && l2->next==NULL)
					l1->next=new ListNode(1);
				else
					l1->next=l1->next?addcarry(l1->next):addcarry(l2->next);
			}
			return tmp;
		}
};

int main(){
	class Solution *s = new Solution();
	/*ListNode *l1 = new ListNode(2);
	l1->next = new ListNode(4);
	l1->next->next = new ListNode(3);	
	ListNode *l2 = new ListNode(5);
	l2->next = new ListNode(6);
	l2->next->next = new ListNode(6);
	l2->next->next->next=new ListNode(9);
	l2->next->next->next->next=new ListNode(9);
	l1 = s->addTwoNumbers(l1,l2);*/
	ListNode *l1 = new ListNode(5);
	ListNode *l2 = new ListNode(5);
	l1 = s->addTwoNumbers(l1,l2);
	while(l1){
		cout<<l1->val<<endl;
		l1=l1->next;
	}
	return 0;
}
