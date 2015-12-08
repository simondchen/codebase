#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include  <sys/wait.h>

sem_t *semr;
sem_t *semw;
int main(void)
{
	int shmid;
	char *c1_addr, *c2_addr;
	char *name = "./myshm";
	//char content[8] = "haha";
	char buf[16];
	key_t key;
	FILE *fp;
	
	semr = sem_open("mysem_ra",  O_CREAT | O_EXCL | O_RDWR , 0666, 0);
    if (semr == SEM_FAILED)
    {
        perror("sem_open myserm_r");
        //printf("errno=%d\n", errno);
        exit(1);
    }
    semw = sem_open("mysem_wa",  O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if (semw == SEM_FAILED)
    {
        perror("sem_open myserm_r");
        //printf("errno=%d\n", errno);
        exit(1);
    }
    int sval=5;
    if(sem_getvalue(semw,&sval)<0)
        perror("sem_getvalue error");
    printf("sem value:%d\n",sval);
	key = ftok(name, 0);
	if(key == -1)
		perror("ftok error");
	shmid = shmget(key, 65535, 0777|IPC_CREAT);
	if(shmid == -1)
	{
		perror("Create Share Memory Error:");
		exit(1);
	}
    pid_t pid;
	if((pid=fork())<0){
        perror("fork error");
        exit(-1);
    }
	else if(pid>0){
		if((pid=fork())<0){
            perror("fork error");
            exit(-1);
        }
		else if(pid>0){
			//parent process
			sleep(1);
		}
		else
		{
			//child 2 process read
			printf("2\n");
			sleep(2);
			c2_addr = shmat(shmid, 0, 0);
			//while(1)
			//{			
				sem_wait(semr);			
				printf("get %s", c2_addr);
				printf("\n");
				sem_post(semw);
				sleep(1);
                sem_close(semr);
                sem_close(semw);
			//}
		}
	}
	else
	{
		//child 1 process	write
		printf("1\n");
		c1_addr = shmat(shmid, 0, 0);
	    memset(c1_addr, 0, 1024);
		fp = fopen("sample.txt", "r");
		//fscanf(fp, "%s", buf);
		//printf("%s", buf);
		/*while(!feof(fp))
		{*/
            //if(sem_trywait(semw)<0)
                //perror("sem_trywait error");
			sem_wait(semw);
			fscanf(fp, "%s", buf);
			strncpy(c1_addr, buf, 16);
			sem_post(semr);
			sleep(1);
		//}
	}
    printf("parent\n");
    if(sem_close(semr)<0)
        perror("sem_close semr error");
    if(sem_close(semw)<0)
        perror("sem_close semr error");
	wait(NULL);
	return 0;
}

