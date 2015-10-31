#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(void){
	struct winsize ws;
	int tty_in = open("/dev/tty",O_RDONLY);
	if(tty_in<0){
		printf("open tty error\n");
		return -1;
	}
	if(ioctl(tty_in,TIOCGWINSZ,&ws)<0){
		printf("ioctl error\n");
		return -1;
	}
	printf("rows:%d,columns:%d\n",ws.ws_row,ws.ws_col);
	printf("x_pixel:%d,y_pixel:%d\n",ws.ws_xpixel,ws.ws_ypixel);
	return 0;
}
