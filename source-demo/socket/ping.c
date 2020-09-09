#include<stdio.h>
#include<sys/types.h>
int go_ping(char *svrip)
{
        int i = 0;
        while(i < 3)
        {
                pid_t pid;
                if ((pid = vfork()) < 0) 
                {
                        printf("vfork error");
                        exit(1);
                } 
                else if (pid == 0) 
                {
                        if ( execlp("ping", "ping","-c 1",svrip, (char*)0) < 0)
                        {
                                printf("execlp error\n");
                                exit(1);
                        }
                }

                int stat;
                waitpid(pid, &stat, 0);

                if (stat == 0)
                {
                        return 0;
                }
                //sleep(1);
                i++;
        }
        return -1;
}

void main(int argc, char **argv)
{
	char *ip = "8.8.8.8";
	if(go_ping(ip) == 0){
		printf("network :%s is ok!\n",ip);
	}else{
		printf("network :%s is error!!!!!!!!!!!\n",ip);
	}
}
