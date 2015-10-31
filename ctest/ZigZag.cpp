/*
 *
 *这题又折腾了好久，一开始是内存超出限制，原因是使用了太多的string+=的格式，这种方式是很耗内存的，因为每一次都要新建一个string
 *然后第二次是因为没有考虑nRows==1的特殊情况，属于考虑不够周全
 *第三次是因为构造的char数组没有以'\0'结尾，导致构造的字符串出问题，这个问题可真难发现哪，原因还是因为对于c++的string不熟悉!!!
 *
 */
#include <iostream>
using namespace std;

class Solution{
	public:
		string convert(string s,int nRows){
			int i;
			int len = s.size();
			cout<<len<<endl;
			//string result = "";
			char result[len+1];
			int num=0;
			//nRows==1时会有问题，因为nRows==1时会使stpe1===0
			if(nRows==1){
				return s;
			}
			for(i=1;i<=nRows;i++){
				if(i>len)
					break;
				int step1=2*(nRows-i);	//4
				int step2=2*(i-1);      //0
				int sw=1;
				int position=i;
				if(i==1)
					sw=3;
				if(i==nRows)
					sw=4;
				while(position<=len){
					//应该放在更改之前！！！
					//result+=s[position-1];
					result[num++]=s[position-1];
					switch(sw){	
						case 1:
							position+=step1;
							sw=2;
							break;
						case 2:
							position+=step2;
							sw=1;
							break;
						case 3:
							position+=step1;
							break;
						case 4:
							position+=step2;
							break;
						default:
							break;
					}
				}
			}			
			result[len]='\0';
			string ss(result);
			return ss;
		}
};

int main(){
	Solution *sl = new Solution(); 	
	string s="";
	s=sl->convert("A",2);
	cout<<s<<endl;
}





