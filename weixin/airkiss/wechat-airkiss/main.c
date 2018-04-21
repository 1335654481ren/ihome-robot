#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#include "airkiss.h"

airkiss_config_t akconf = {
	(airkiss_memset_fn)&memset,
	(airkiss_memcpy_fn)&memcpy,
	(airkiss_memcmp_fn)&memcmp,
	(airkiss_printf_fn)&printf 
};

airkiss_context_t akcontex;

int count = 0;
void timer_task()
{
	airkiss_change_channel(&akcontex);
    printf("count is %d\n", count++);
}

void init_sigaction()
{
    struct sigaction act;
          
    act.sa_handler = timer_task; //设置处理信号的函数
    act.sa_flags  = 0;

    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);//时间到发送SIGROF信号
}

void init_time()
{
    struct itimerval val;
         
    val.it_value.tv_sec = 0; //1秒后启用定时器
    val.it_value.tv_usec = 100*1000;

    val.it_interval = val.it_value; //定时器间隔为1s

    setitimer(ITIMER_PROF, &val, NULL);
}
void uninit_time()    
{    
    struct itimerval value;    
    value.it_value.tv_sec = 0;    
    value.it_value.tv_usec = 0;    
    value.it_interval = value.it_value;    
    setitimer(ITIMER_PROF, &value, NULL);    
}

int main(int argc, char **argv)
{
	int err = 0;
	printf("app ok\n");
	printf("Airkiss verison :%s\n",airkiss_version());
   
    init_sigaction();
    init_time();

	if ( airkiss_init(&akcontex, &akconf) < 0 )
	{
		printf("init airkiss fail！！\n");
	}
	else{
		printf("init airkiss ok\n");
	}
	while(1)
	{
		err = airkiss_recv( &akcontex, const void* frame, unsigned short length);
		if( err == AIRKISS_STATUS_CONTINUE )
		{
			printf("contunue to recv\n");
		}else if( err == AIRKISS_STATUS_CHANNEL_LOCKED ){
			printf("have bind to singal channel,cancel change singal channel\n");
			uninit_time();
		}else if( err == AIRKISS_STATUS_COMPLETE ){
			printf("sucuessful\n");
			if(airkiss_get_result(&akcontex, &akconf) < 0 )
			{
				printf("get result fail!\n");
			}
			else{
				printf("get result ok\n");
			}
		}
	}
	return 0;
}
