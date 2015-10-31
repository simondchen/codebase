%
%2015年 10月 20日 星期二 13:54:49 CST
%<nicksimon109445@gmail.com>
%draw the rss samples
%
%function draw_seq(fname1,fname2)
function draw_seq()
    data=load('seq_final_alice')
    data=data'
    x=1:size(data,2)
    y1=data(x)
    plot(x,y1)
    data=load('seq_final_bob')
    data=data'
    x=1:size(data,2)
    y2=data(x)
    hold on
    plot(x,y2)
end