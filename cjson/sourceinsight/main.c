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

unsigned short get_rand_misgID()
{
	struct timeval tpstart;
	int n = 100;

	gettimeofday(&tpstart,NULL);
	srand(tpstart.tv_usec);
	n = (1+(int) (3000.0*rand()/(RAND_MAX+1.0)));
	printf("get_rand_misgID = 0x%04x \n",n);
	return (unsigned short)n;
}

void send_ctrl_msg(char msg_type,char *json_buf, int len)
{
	char send_buff[512] = {0x00};
	int index = 0;
	int i;
	unsigned short mig_id = 0;
	mig_id = get_rand_misgID();
	msg_type = 0x05;//main ctrl msg type
	
	send_buff[0] = 0xA5;
	send_buff[1] = 0x01;
	send_buff[2] = msg_type;  		//msg type tts=5
	send_buff[3] = (char)len;			//msg lenth
	send_buff[4] = (char)(len>>8);
	send_buff[5] = (char)mig_id;    	//msg id
	send_buff[6] = (char)(mig_id>>8);
	memcpy(send_buff+AIUI_FRAM_HEAD_COST,json_buf,len);
	len = len + AIUI_FRAM_HEAD_COST;
	
	//make check code
	char check_code = 0;
	for(index = 0; index <= len; index++){
		check_code += send_buff[index];
	}
	check_code = ~check_code + 1;
	send_buff[len++] = check_code;
	printf("fram hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",send_buff[i]);
	}
	printf("\n");
	//send data to uart
	uart_send(uart_hd, send_buff, len);

}

//tts
int send_tts(char *strbuff)
{
	int i,len;
	char *pjson = NULL;
/*
{
        "type": "tts",
        "content": {
                "action": "start",  //action
                "text": "xxx"       //the text to speek
        }
}
*/
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("tts"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "action", cJSON_CreateString("start"));
	cJSON_AddItemToObject(item, "text", cJSON_CreateString(strbuff));
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
	send_ctrl_msg(0x05,pjson, len);
    return 0;
}

#define UART_DEVICE "/dev/ttyAMA2"

int main(int argc, char* argv[])
{
	int ret = 0;
	char bugff[4096] = {"我是任晓亮"};
	ret = uart_init(&uart_hd, UART_DEVICE,115200, uart_rec, NULL);
	if (0 != ret)
	{
		printf("uart_init error ret = %d\n", ret);
	}
	while(1)
	{   
		printf("input word:\n");
	//	scanf("%s",bugff);
		send_tts(bugff);
		sleep(15);
	}
}
