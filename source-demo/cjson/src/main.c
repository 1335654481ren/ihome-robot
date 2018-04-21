#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <termios.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include "cJSON.h"
#include "aiui.h"
#include "uart.h"

extern UART_HANDLE uart_hd;

#define UART_DEVICE "/dev/ttyUSB0"

int main(int argc, char* argv[])
{
	int ret = 0;
//	char bugff[4096] = {"肖东红，给你讲个故事，哈哈，哈，完了 ，哈哈，完了，你好我是叮咚叮咚,吃葡萄不吐葡萄皮"};
	
	char bugff[4096] = {"肖东红，给你讲个笑话,哈哈哈，儿子中考考试考差了，被老婆骂了一顿。我去安慰儿子：“你要努力学习，以后一定要超越爸爸。”儿子愣了一下，弱弱来了一句：“别的我不敢保证。但是，以后找个比你好的老婆还是很有把握的。”????"};
	ret = uart_init(&uart_hd, UART_DEVICE,115200, uart_rec, NULL);
	if (0 != ret)
	{
		printf("uart_init error ret = %d\n", ret);
	}
	while(1)
	{   
	//	printf("input word:\n");
	//	scanf("%s",bugff);
	//	AiuiCtrl_tts_start(bugff);
		//sleep(2);
		//AiuiCtrl_Voice_EN(DISABLE);
	//	AiuiCtrl_get_wifi_status();
		sleep(20);
	}
}
