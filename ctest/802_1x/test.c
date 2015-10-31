#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdint.h>

int main(){
	uint8_t body[2];
	body[0]=0x0d;
	body[1]=0x00;
	htons(body);
	printf("%d\n",body[0]);
	return 0;
}
