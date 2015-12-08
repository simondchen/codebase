package com.simon.candy;

import java.util.*;

public class Candy {
	
	/*
	 * 分析:
	 * 这题应该是用到极大值和极小值
	 * 找出所有极小值和极大值,比较极大值和旁边的两个极小值的距离,每个极小值从1开始取
	 * 极大值由距离较远者决定
	 * 极大值和极小值的判断
	 * a[i-1]>a[i]<a[i+1]极小值
	 * a[i-1]<a[i]>a[i+1]极大值
	 */
	public int noeqcandy(int[] ratings,int l,int r){
		int ret=0;
		//没有相等点的情况!!!
		ArrayList<Integer> max=new ArrayList<>();
		ArrayList<Integer> min=new ArrayList<>();
		/*
		 * 标准化!!!
		 * 补一个极小点,之前想得是补l-1,但如果补l的话,可以神奇的统一起来,而且由于距离为0不会影响结果!!!
		 */
		min.add(l);
		if(ratings[l]>ratings[l+1]){
			//左高情形
			max.add(l);
		}
		for(int i=l+1;i<r;i++){
			int lf=ratings[i-1],rg=ratings[i+1],md=ratings[i];
			if(md>lf && md>rg){
				max.add(i);
			}else if(md<lf && md<rg){
				min.add(i);
			}
		}
		
		min.add(r);
		if(ratings[r]>ratings[r-1]){
			//左高情形
			max.add(r);
		}
		
		//现在是标准的两端低的情形
		for(int i=0;i<max.size();i++){
			int lmin=min.get(i);
			int rmin=min.get(i+1);
			int mmax=max.get(i);
			int llen=mmax-lmin;
			int rlen=rmin-mmax;
			int m,n;
			if(llen>rlen){
				n=llen+1;
				m=rlen;
			}else{
				n=rlen+1;
				m=llen;
			}
			ret+=n*(n+1)/2;
			ret+=m*(m+1)/2;
		}
		return ret-min.size()+2;
	}
	 /* 要注意相等的情况!!!
	 * 这题相等的情况的处理才是难点,之前的思路都是没考虑相等的,所以将那个单独写成一个
	 * 函数,而单独处理相等的情况,主要由两种情况
	 * ①相等的点为2
	 * 	断层:即将以两个相等的点为界一分为二
	 *  对每一组分别调用上面的函数
	 * ②相等的点>=3
	 * 	挖出这些点
	 */
	public int candy(int[] ratings) {
		int total=0;
		int len=ratings.length;
		//处理特殊情形
		if(len==1)
			return 1;
		if(len==2)
			return ratings[0]==ratings[1]?2:3;
		ArrayList<Integer> bp=new ArrayList<Integer>();
		/*
		 * 把初始点和末点添加进来,维持统一性,这相当于数学定义中,
		 * 对某些特殊点单独定义以下,以维持数学定义在所有定义域上的正确性
		 */
		if(ratings[0]==ratings[1]){
			total++;
		}else{
			bp.add(0);
		}
		for(int i=1;i<len-1;i++){
			int l=ratings[i-1],r=ratings[i+1],m=ratings[i];
			if(m==l && m==r){
				//对于和两端都相等的点,直接赋值为1,并去掉
				total++;
			}else if(m==l || m==r){
				bp.add(i);
			}
		}
		if(ratings[len-1]==ratings[len-2]){
			total++;
		}else{
			bp.add(len-1);
		}
		
		Iterator<Integer> it=bp.iterator();
		while(it.hasNext()){
			total+=noeqcandy(ratings,it.next(),it.next());
		}
		
		return total;
    }

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Candy candy=new Candy();
		int ratings[]={1,1,3,3,3,5,3,7,7};
//		System.out.println(candy.noeqcandy(ratings,0,4));
		System.out.println(candy.candy(ratings));
	}

}
