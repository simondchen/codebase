package com.simon.single_number3;

import java.math.*;

public class Single_Number3 {

	public int log_len(int num){
		for(int i=0;;i++){
			System.out.println(i);
			if((num>>i)==0)
				return i;
		}
	}
	
	public int singleNumber(int[] nums) {
//		int k=1;
//		int off=0;
//		int sum=0;
//		int ret=0;
//		
//		do{
//			sum=0;
//			for(int i=0;i<nums.length;i++){
//				//只可能为3k或3k+1
//				sum+=(nums[i]&(k<<off))>>off;
//			}
//			ret|=(sum%3)<<off;
//			off++;
//		}while(sum!=0);
//        return ret;
		//处理负数的情况,取绝对值
//		int total=0;
//		for(int i=0;i<nums.length;i++){
//			total+=nums[i];
//			nums[i]=Math.abs(nums[i]);
//		}
		//首先应找出最大数,以确定外层循环次数
//		int max=nums[0];
//		for(int i=1;i<nums.length;i++){
//			if(nums[i]>max)
//				max=nums[i];
//		}
//		int len=log_len(max);
		int ret=0;
		for(int off=0;off<32;off++){
			int k=1<<off;
			int sum=0;
			for(int i=0;i<nums.length;i++){
				//sum为3k或3k+1
				//!!!这里要使用逻辑右移
				sum+=(nums[i]&k)>>>off;
			}
			int m=sum%3;
			ret|=(m==0?0:1<<off);
		}
		return ret;
    }
	
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Single_Number3 sn=new Single_Number3();
		int nums[] = {-4,1,3,-4,2,1,3,-4,1,3,2,-5,2};
		System.out.println(sn.singleNumber(nums));
		int i=-1;
		Integer in=new Integer(i);
		long b=i;
		System.out.println(b);
	}

}
