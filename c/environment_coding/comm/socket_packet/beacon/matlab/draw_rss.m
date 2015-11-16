%
%2015年 10月 20日 星期二 13:54:49 CST
%<nicksimon109445@gmail.com>
%draw the rss samples
%
%function draw_seq(fname1,fname2)
function draw_seq()
    data=load('seq_initiator')
    data=data'
    %x=1:size(data,2)
    x=250:350
    y1=data(x)
    plot(x,y1,'-o')
    data=load('seq_responser')
    data=data'
    %x=1:size(data,2)
    x=250:350
    y2=data(x)
    hold on
    plot(x,y2,'-o')
end
