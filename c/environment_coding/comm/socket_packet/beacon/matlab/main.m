%
%simon <nicksimon@gmail.com>
%2015-11-12 16:12
%

%draw_rss();
global m;
m=1;
data=load('seq_initiator');
data=data';
%x=1:size(data,2);
x=250:350
y1=data(x);
data=load('seq_responser');
data=data';
%x=1:size(data,2);
x=250:350
y2=data(x);
generate_key(y1,y2);
ans
clear x data;