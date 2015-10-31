#include <stdio.h>
#include <stdlib.h>

struct hide{
	int flag;
};

struct hide *construct(void){
	struct hide *hide = malloc(sizeof(struct hide));
	hide->flag = 0;
	return hide;
}

void setflag(struct hide *hide,int mode){
	hide->flag |= mode;
}

int getflag(struct hide *hide){
	return hide->flag;
}
