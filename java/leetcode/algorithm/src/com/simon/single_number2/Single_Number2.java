package com.simon.single_number2;

import java.util.*;

public class Single_Number2 {

	/*
	 * 还是利用异或,之前如果只有一个单独数,则直接异或就可以得到结果
	 * 现在有两个异或数,直接异或得到a^b
	 * 所以想法是如果能够将数据分成两组,一组只含a,一组只含b,那么这两组分别异或,就可得出结果!!!
	 * 注意a^b得到了a和b不同的位!!!这个是二者的差异,所以利用这个特点来进行分组!!!
	 * 思维,逻辑
	 */
	public int[] singleNumber(int[] nums){
		int output=0;
		//计算a^b
		for(int i=0;i<nums.length;i++)
			output^=nums[i];
		//求出某一位不为1的
		int k=1;
		int off=0;
		while(true){
			if((output&(k<<off))!=0){
				break;
			}
			off++;
		}
		k<<=off;
		int[] ret=new int[2];
		for(int i=0;i<nums.length;i++){
			if((nums[i]&k)==0)
				ret[0]^=nums[i];
			else
				ret[1]^=nums[i];
		}
		
		
		return ret;
	}
	
	//利用hashset的方法并不满足空间的constant!!!
//	public int[] singleNumber(int[] nums) {
//		HashSet<Integer> hs=new HashSet<Integer>();
//		for(int i=0;i<nums.length;i++){
//			if(hs.contains(nums[i])){
//				hs.remove(nums[i]);
//			}else{
//				hs.add(nums[i]);
//			}
//		}
//		
//		int len=hs.size();
//		int[] ret=new int[len];
//		int i=0;
//		Iterator<Integer> it=hs.iterator();
//		while(it.hasNext()){
//			ret[i++]=it.next();
//		}
//		return ret;
//	}
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Single_Number2 sn=new Single_Number2();
		int[] nums={1,2,1,3,2,5};
		int[] ret=sn.singleNumber(nums);
		for(int i=0;i<ret.length;i++)
			System.out.println(ret[i]);
	}

}
