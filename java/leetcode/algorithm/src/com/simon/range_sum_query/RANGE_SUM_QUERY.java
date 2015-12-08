package com.simon.range_sum_query;

import java.math.BigInteger;
import java.util.*;

import javax.naming.BinaryRefAddr;

public class RANGE_SUM_QUERY {

	public Map<Integer,ArrayList<Integer>> list=new HashMap<Integer,ArrayList<Integer>>();
	
	public int log_len(int len){
		int i=0;
		while(Math.pow(2,i)<len){
			i++;
		}
		return i;
	}
	
	public RANGE_SUM_QUERY(int[] nums) {
        //将nums的长度补全成2的平方数,方便操作
		int i;
		int len=nums.length;
		int loglen=log_len(len);
		int newlen=(int)Math.pow(2,loglen);
		//将nums添加到list中去
		ArrayList<Integer> l=new ArrayList<>();
		for(i=0;i<len;i++){
			l.add(nums[i]);
		}
		for(i=len;i<newlen;i++){
			l.add(0);
		}
		list.put(0,l);
		//初始化,计算2^0,2^1,2^2,...2^newlen的长度的部分和
		for(i=1;i<=loglen;i++){
			ArrayList<Integer> locallist=list.get(i-1);
			ArrayList<Integer> newlist=new ArrayList<Integer>();
			for(int j=0;j<locallist.size();j+=2){
				newlist.add(locallist.get(j)+locallist.get(j+1));
			}
			list.put(i,newlist);
		}
    }
 
	
    public int sumRange(int i, int j) {
    	int sum=0;
    	//使用之前计算出来的部分和计算,而不直接计算i到j的部分和
    	int k=j-i+1;
    	int t=log_len(k);
    	
    	int l=(int)Math.pow(2,t);
    	int p=0;
    	if(l==k){
    		//len正好是2的指数次幂
    		p=t;
    	}else{
    		p=t-1;
    	}
    	int m=(int)Math.pow(2,p);
    	//想复杂了,直接(i/m)+1即可
    	int q=0;
    	if(i%m==0){
    		q=i;
    	}else{
    		q=(i/m+1)*m;
    	}
    	int tmp=q-1;
    	int mp=p;
    	//向左延伸
    	int llen;
    	for(llen=m;llen>=1;llen/=2,mp--){
    		int slen=tmp-i+1;
    		if(slen>=llen){
    			sum+=list.get(mp).get(tmp/llen);
    			tmp-=llen;
    		}
    	}
    	//向右延伸
    	int rlen;
    	tmp=q;
    	mp=p;
    	for(rlen=m;rlen>=1;rlen/=2,mp--){
    		int slen=j-tmp+1;
    		if(slen>=rlen){
    			sum+=list.get(mp).get(tmp/rlen);
    			tmp+=rlen;
    		}
    	}
        return sum;
    }
	
	
	
	public static void main(String[] args) { 
		// TODO Auto-generated method stub
		int nums[]={-2, 0, 3, -5, 2, -1};
//		System.out.println(nums);
		RANGE_SUM_QUERY rsq=new RANGE_SUM_QUERY(nums);
		System.out.println(rsq.list);
		System.out.println(rsq.sumRange(4,7));
	}

}
