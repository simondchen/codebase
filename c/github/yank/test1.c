#include <stdio.h>
int main(int argc,char **argv)
{
	
	printf("\033[31mThe color,%s!\033[1m\n","haha");
	
	printf("\033[31mThe color,%s!\033[4m\n","haha");
	
	printf("\033[31mThe color,%s!\033[5m\n","haha");
	
	printf("\033[31mThe color,%s!\033[7m\n","haha");
	
	printf("\033[31mThe color,%s!\033[8m\n","haha");
		
	
	
	printf("\033[31mThe color,%s!\033[0m\n","haha");
	
	printf("\033[47;31mThe color,%s!\033[0m\n","haha");
	
	printf("\033[47mThe color,%s!\033[0m\n","haha");
	
	sleep(2);
	
	printf("\033[47m%s!\033[5A\n","up 5");
	
	sleep(2);
	
	printf("\033[47m%s!\033[9B\n","down 9");
	
	sleep(2);
	
	printf("\033[47m%s!\033[19C\n","right 19");
	
	printf("right19");
	
	sleep(2);
	
	printf("\033[47m%s!\033[10D\n","left 10");
	
	printf("left 10");
	
	sleep(2);
	
	printf("\033[47m%s!\033[50;20H\n","move to y:50,x 20");
	
	printf("y50 x 20");
	
	sleep(2);
	
	printf("\033[47m%s!\033[?25l\n","hide cursor");
	
	sleep(2);
	
	printf("\033[47m%s!\033[?25h\n","sow cursor");
	
	sleep(2);
	
	printf("\033[47m%s!\033[2J\n","clear scleen");
	
	sleep(2);
}
