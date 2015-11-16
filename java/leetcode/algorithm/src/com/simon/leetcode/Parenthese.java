package com.simon.leetcode;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Parenthese {

	public List<String> ltr(String s){
		int i;
		//记录新一轮的搜索开始的位置,其中cur之前的认为是没有问题的
		int cur=0;
//		StringBuffer sb=new StringBuffer(s);
		Deque<Integer> queue=new ArrayDeque<>();
		//存放')'出现的位置
		List<Integer> l=new ArrayList<Integer>();
		//存放每部分修复的几种方案
		ArrayList<HashSet<String>> ll=new ArrayList<HashSet<String>>();
		//!!!循环不变式,i之前的部分是no problem的,我觉得时间复杂度应该是n^2,不是指数,cur指向没有问题的下一位
		for(i=0;i<s.length();i++){
			char c=s.charAt(i);
			switch(c){
				//将'('所在位置入栈
				case '(':
					queue.push(i);
					break;
				case ')':
					l.add(i);
					if(queue.isEmpty()){
						//若栈为空,说明出错,进行repair操作,repair之后相当于新一轮的开始,清空l
						//start repair
						//这里repair是去掉一个')',但是去掉哪一个')',如何判断重复是难点,这里简单的使用遍历比较相等的方式去重
						HashSet<String> part=new HashSet<String>();
 						for(int pos:l){
 							StringBuffer sb=new StringBuffer(s.substring(cur,i));
 							sb.deleteCharAt(pos);
 							part.add(sb.toString());
						}
 						//重新设置cur,并清l
 						cur=i+1;
 						l.clear();
						//end repair
					}else{
						//若栈不为空,出栈,同时记录该')'所在位置
						queue.pop();
						//推进no problem的边缘
						if(queue.isEmpty()){
							
						}
						//更新cur
						cur=i+1;
					}
					break;
				default:
					break;
			}
		}
		return null;
	}
	
	public List<String> rtl(String s){
		return null;
	}
	
	
	public List<String> removeInvalidParentheses(String s) {
		
		
		
		//若栈中还有括号,说明是多余的括号,
		
        return null;
    }
	
	
	public static void main(String[] args){
		List<String> hv=new ArrayList<String>();
		StringBuffer sb=new StringBuffer("abc");
		StringBuffer tmp=sb;
		tmp.deleteCharAt(0);
		tmp.deleteCharAt(1);
		System.out.println(sb);
		System.out.println(tmp);
	}
	
}
