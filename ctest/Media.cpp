#include <iostream>
using namespace std;
	
struct media{
	double median;
	int pos;
};

struct media singlemedia(int a[],int la,int lb){ 
	int len=lb-la+1; 
	struct media m;
	if(len%2){
		m.pos=(lb+la)/2-1;
		m.median=a[(lb+la)/2-1]; 
		return m;
	}
	else{
		m.pos=(lb+la-1)/2;
		m.median=(double)(a[(lb+la-1)/2-1]+a[(lb+la+1)/2-1])/2;
		return m;
	}
}

//array begin from 1!!!
int oddmedia(int a[],int la,int ra,int b[],int lb,int rb,int direct){
	int index=0;
	struct media m=singlemedia(a,la,ra); 
	if(la==ra)
			
	if(direct==0){ //shift right
		for(index=lb;index<=rb;index++){	
			if(b[index-1]>=m.median){
				int num=m.pos+index-1;
				if(num==ll){
					return a[m.pos+1]>=b[index-1]?b[index-1]:a[m.pos+1];
				}
				else if(num==lr){
					if(index==lb)
						return a[m.pos];
					else
						return a[m.pos]<=b[index-2]?b[index-2]:a[m.pos];
				}
				else if(num<ll){
					la=m.pos+1;
					lb=index;
					direct=0;
					break;
				}else{
					ra=m.pos;
					rb=index-1;
					direct=1;
					break;
				}
			}
		}
		if(index>rb){
			la=m.pos;	
			rb=rb;
			direct=1;
		}
	}else{
		for(index=rb;index>=lb;index--){
			if(b[index-1]<=m.median){
				int num=m.pos+index;
				if(num==ll){
					if(index==rb)
						return a[m.pos+1]; 
					else
						return a[m.pos+1]>=a[index]?a[index]:a[m.pos+1];
				}else if(num==lr){
					return a[m.pos]<=a[index-1]?a[index-1]:a[pos];
				}else if(num<ll){
					la=m.pos+1;
					lb=index+1;
					direct=0;
				}
			}
		}
	}
	if(lb==rb)
		return 
	return 0;		
}

double doumedia(int a[],int la,int ra,int b[],int lb,int rb){
	return 0;
}

int main(){
	int a[]={1,3,6,7,8};
	struct media m = singlemedia(a,3,5); 
	cout<<m.median<<endl;
	cout<<m.pos<<endl;
	return 0;
}
