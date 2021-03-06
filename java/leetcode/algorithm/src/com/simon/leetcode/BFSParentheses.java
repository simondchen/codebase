package com.simon.leetcode;

import java.util.*;
public class BFSParentheses {

	//定义一个node类
	class Node{
		String s;
		int depth;
		public Node(String s,int depth){
			this.s=s;
			this.depth=depth;
		}
	}
	
	//检测是否合法:合法 0,不合法 -1
	public int checkValid(String s){
		Deque<Integer> q=new ArrayDeque<>();
		for(int i=0;i<s.length();i++){
			char c=s.charAt(i);
			switch(c){
				case '(':
					q.push(i);
					break;
				case ')':
					if(q.isEmpty()){
						return -1;
					}else{
						q.pop();
					}
					break;
				default:
					break;
			}
		}
		return q.isEmpty()?0:-1;
	}
	
	public List<String> removeInvalidParentheses(String s) {
		//由于是去掉最少的括号,所以以去掉括号的个数为BFS的层数.通过记录的方式减少不必要的访问(重复的访问子树),缩短时间的关键是备忘memorized
		//标记在某一level是否已经找到一个合法的string,且其值表示在哪一level找到
		int flag=-1;
		//全局hashset,用作去重
		Set<String> gset=new HashSet<String>();
		Set<String> output=new HashSet<String>();
		//初始化BFS的队列Q
		Deque<Node> q=new ArrayDeque<Node>();
		Node root=new Node(s,0);
		q.addLast(root);
		
		while(!q.isEmpty()){
			Node u=q.removeFirst();
			String subs=u.s;
			//说明已经找到全部,返回结果即可,这里很巧妙
			if((u.depth==(flag+1)) && flag!=-1){
				break;
			}
			if(checkValid(subs)==0){
				//说明合法,结束本次循环,并不检测深一层的子树,但要检测完同一level的所有节点
				if(flag==-1)
					flag=u.depth;
				output.add(subs);
				continue;
			}else{
				//u.s无效,分两种情况讨论:a.flag==-1,说明还没找到合适的,所以要将子树都填加 b.flag!=-1,说明在该level已经找到,不必再添加子树
				if(flag!=-1){
					continue;
				}else{
					for(int i=0;i<subs.length();i++){
						StringBuilder sb=new StringBuilder(subs);
						sb.deleteCharAt(i);
						String ssubs=sb.toString();
						if(!gset.contains(ssubs)){
							gset.add(ssubs);
							Node n=new Node(ssubs,u.depth+1);
							q.addLast(n);
						}
					}
				}
			}
		}

		
		
        return new ArrayList<String>(output);
    }
	
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		BFSParentheses bp=new BFSParentheses();
		ArrayList<String> l=(ArrayList<String>) bp.removeInvalidParentheses("(()()()))");
		for(String s:l){
			System.out.println(s);
		}
	}

}
