package com.simon.single_number;

public class Single_Number {

	
	
	/*
	 * 要求:①线性时间 ②不占用额外空间
	 */
	
	/*
	 * 思路:一开始想着用hash,虽然是线性时间,但是要用额外空间
	 * 		不小心看到discuss中说用XOR,衰,不过真是个好办法!!!
	 */
	public int singleNumber(int[] nums) {
		int output=0;
		for(int i=0;i<nums.length;i++){
			output^=nums[i];
		}
		
        return output;
    }
	
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Single_Number sn=new Single_Number();
		int nums[]={1,3,4,1,4,5,7,7,5};
		System.out.println(sn.singleNumber(nums));
	}

}
