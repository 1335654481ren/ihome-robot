#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#include "uart.h"
#include "aiui.h"
#include "cJSON.h"

extern UART_HANDLE uart_hd;
static char ack_buf[RECV_BUF_LEN];

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
//	int i;
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
/*	
	printf("fram hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",send_buff[i]);
	}
	printf("\n");
*/
	//send data to uart
	uart_send(uart_hd, send_buff, len);

}

//tts
int AiuiCtrl_tts_start(char *strbuff)
{
	int len;
//	int i;
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
	
/*	printf("%s\n", cJSON_Print(root));
	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
*/
	send_ctrl_msg(0x05,pjson, len);
    return 0;
}

//tts
int AiuiCtrl_tts_stop()
{
	int len;
//	int i;
	char *pjson = NULL;
/*
{
        "type": "tts",
        "content": {
                "action": "stop",  //action
        }
}
*/
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("tts"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "action", cJSON_CreateString("stop"));
	
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
/*	printf("%s\n", cJSON_Print(root));
	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
*/
	send_ctrl_msg(0x05,pjson, len);
    return 0;
}

//wifi status
int AiuiCtrl_get_wifi_status()
{
	int len;
//	int i;
	char *pjson = NULL;
/*
{
        "type": "tts",
        "content": {
                "action": "stop",  //action
        }
}
*/
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("status"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "query", cJSON_CreateString("wifi"));
	pjson = cJSON_Print(root);
	len = strlen(pjson);

/*	printf("%s\n", cJSON_Print(root));
	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
*/
	send_ctrl_msg(0x05,pjson, len);
    return 0;
}

int AiuiCtrl_Voice_EN(int status)
{
	int len;
//	int i;
	char *pjson = NULL;
/*
{
        "type": "tts",
        "content": {
                "action": "stop",  //action
        }
}
*/ 
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("voice"));
    cJSON_AddItemToObject(root, "content", item);
	if(status == ENABLE)
	{
    	cJSON_AddItemToObject(item, "voice_enable",cJSON_CreateTrue());
	}
	else if(status == DISABLE)
	{
		cJSON_AddItemToObject(item, "voice_enable",cJSON_CreateFalse());
	}
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
/*	printf("%s\n", cJSON_Print(root));
	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
*/
	send_ctrl_msg(0x05,pjson, len);
    return 0;
}


void process_recv(unsigned char* buf, int len)
{
	if(buf[2] == 0xff){
//		printf("get ack msg:%d ,0x%04x\n",buf[2],(unsigned short)(buf[6]<<8)|buf[5]);
		//ack msg
		return;
	}

	int index;
	ack_buf[0] = 0xA5;
	ack_buf[1] = 0x01;
	ack_buf[2] = 0xff; //ack msg type :0xff
	ack_buf[3] = 0x04;
	ack_buf[4] = 0x00;
	ack_buf[5] = buf[5]; //recv msg id to ack msg id
	ack_buf[6] = buf[6];
	ack_buf[7] = 0xA5;
	ack_buf[8] = 0x00;
	ack_buf[9] = 0x00;
	ack_buf[10] = 0x00;

	//make check code
	char check_code = 0;
	for(index = 0; index <= 10; index++){
		check_code += ack_buf[index];
	}
	check_code = ~check_code + 1;
	ack_buf[11] = check_code;

	//send data
	uart_send(uart_hd, ack_buf, 12);
/*
	printf("recv ");
	
	for(index = 0; index < len; index++){
		if(index % 16 == 0)
			printf("\n");
		printf("%02X ", buf[index]);
	}
	printf("\n");
*/
//	printf("MsgType:%d\n",buf[2]);
//	printf("MsgID:0x%x\n",(unsigned short)buf[4]<<8 | buf[3]);

	data_to_json(buf+AIUI_FRAM_HEAD_COST, len-AIUI_FRAM_COST);
}

