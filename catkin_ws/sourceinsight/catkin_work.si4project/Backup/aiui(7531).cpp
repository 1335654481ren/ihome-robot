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
//	printf("get_rand_misgID = 0x%04x \n",n);
	return (unsigned short)n;
}

void send_ctrl_msg(char msg_type,char *json_buf, int len)
{
	char send_buff[512] = {0x00};
	int index = 0;
//	int i;
	unsigned short mig_id = 0;
	mig_id = get_rand_misgID();
	
	send_buff[0] = SYNC_HEAD;
	send_buff[1] = USER_ID;
	send_buff[2] = CONTROL_MESSAGE_TYPE;  		//msg type tts=5
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

int AiuiCtrl_Config_wifi(char *ssid,char *passwd)
{
	char send_buff[512] = {0x00};
	int index = 0;
	int ssid_len = strlen(ssid);
	int passwd_len = strlen(passwd);
	int fream_len = 0;
	int i;
	unsigned short mig_id = 0;
	mig_id = get_rand_misgID();
	int msg_len = ssid_len + ssid_len + 4;
	send_buff[fream_len++] = SYNC_HEAD;
	send_buff[fream_len++] = USER_ID;
	send_buff[fream_len++] = WIFI_CONFIG_TYPE;  		//msg type tts=5
	send_buff[fream_len++] = (char)msg_len;			//msg lenth
	send_buff[fream_len++] = (char)(msg_len>>8);
	send_buff[fream_len++] = (char)mig_id;    	//msg id
	send_buff[fream_len++] = (char)(mig_id>>8);
	send_buff[fream_len++] = 0x00; //status
	send_buff[fream_len++] = 0x02; //encrypt 0x00 = none 0x01 = wep 0x02 = wpa
    send_buff[fream_len++] = (char)ssid_len;
    send_buff[fream_len++] = (char)passwd_len;
    for ( i = 0; i < ssid_len; i++)
    {
        send_buff[fream_len++] = ssid[i];
    }
    for ( i = 0; i < passwd_len; i++)
    {
        send_buff[fream_len++] = passwd[i];
    }
	//make check code
	char check_code = 0;
	for(index = 0; index < fream_len; index++)
	{
		check_code += send_buff[index];
	}
	check_code = ~check_code + 1;
	send_buff[fream_len++] = check_code;

    printf("hex data ssid=%d-%s , pwd =%d-%s len =%d:\n",ssid_len,ssid,passwd_len,passwd,fream_len);
	for(i = 0; i < fream_len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",(unsigned char)send_buff[i]);
	}
	printf("\n");

	uart_send(uart_hd, send_buff, fream_len);
}

int AiuiCtrl_smartconfig(bool action)
{
	int len;
//	int i;
	char *pjson = NULL;

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("smartcfg"));
    cJSON_AddItemToObject(root, "content", item);
    if(action == true)
    	cJSON_AddItemToObject(item, "cmd", cJSON_CreateString("start"));
    else
    	cJSON_AddItemToObject(item, "cmd", cJSON_CreateString("stop"));
	cJSON_AddItemToObject(item, "timeout", cJSON_CreateNumber(60));

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


int AiuiCtrl_wakeup(bool isResetWake)
{
	int len;
//	int i;
	char *pjson = NULL;

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("aiui_msg"));
    cJSON_AddItemToObject(root, "content", item);
    if(!isResetWake)
    	cJSON_AddItemToObject(item, "msg_type", cJSON_CreateNumber(8));//sleep
    else
    	cJSON_AddItemToObject(item, "msg_type", cJSON_CreateNumber(7));//wakeup
	cJSON_AddItemToObject(item, "arg1", cJSON_CreateNumber(0));
	cJSON_AddItemToObject(item, "arg2", cJSON_CreateNumber(0));
	cJSON_AddItemToObject(item, "params", cJSON_CreateString(""));
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
//	printf("%s\n", cJSON_Print(root));
/*	printf("json_len:%d\n %s\n",len, pjson);
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

int AiuiCtrl_config(char *appid,char *key,char *scene,bool launch)
{
	int len;
//	int i;
	char *pjson = NULL;

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("aiui_cfg"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "appid", cJSON_CreateString(appid));
	cJSON_AddItemToObject(item, "key", cJSON_CreateString(key));
	cJSON_AddItemToObject(item, "scene", cJSON_CreateString(scene));
	cJSON_AddItemToObject(item, "launch_demo", cJSON_CreateTrue());
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
	printf("%s\n", cJSON_Print(root));
/*	printf("json_len:%d\n %s\n",len, pjson);
    printf("json hex data:\n");
	for(i = 0; i < len; i ++)
	{
		if(i % 16 == 0)
			printf("\n");
		printf("%02x ",pjson[i]);
	}
	printf("\n");
*/
//	send_ctrl_msg(0x05,pjson, len);
    return 0;	
}
//tts
int AiuiCtrl_tts_start(char *strbuff)
{
	int len;
//	int i;
	char *pjson = NULL;

    cJSON * root =  cJSON_CreateObject();
    cJSON_AddItemToObject(root, "type", cJSON_CreateString("tts"));
    cJSON * item =  cJSON_CreateObject();
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

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("tts"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "action", cJSON_CreateString("stop"));
	
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
//	printf("%s\n", cJSON_Print(root));
/*	printf("json_len:%d\n %s\n",len, pjson);
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

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("status"));
    cJSON_AddItemToObject(root, "content", item);
    cJSON_AddItemToObject(item, "query", cJSON_CreateString("wifi"));
	pjson = cJSON_Print(root);
	len = strlen(pjson);

	printf("%s\n", cJSON_Print(root));
/*	printf("json_len:%d\n %s\n",len, pjson);
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
                "action": false/true,  //action
        }
}
*/ 
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "type", cJSON_CreateString("voice"));
    cJSON_AddItemToObject(root, "content", item);
	if(status == ENABLE)
	{
    	cJSON_AddItemToObject(item, "enable_voice",cJSON_CreateTrue());
	}
	else if(status == DISABLE)
	{
		cJSON_AddItemToObject(item, "enable_voice",cJSON_CreateFalse());
	}
	pjson = cJSON_Print(root);
	len = strlen(pjson);
	
//	printf("%s\n", cJSON_Print(root));
/*	printf("json_len:%d\n %s\n",len, pjson);
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

int ask_aiui_todo(unsigned char type,const char *arg1,const char *arg2)
{
	int err = 0;
	char *ssid = (char *) arg1;
	char *passwd = (char *) arg2;
	switch(type)
	{
		case TODO_TTS_START:
			err = AiuiCtrl_tts_start((char*)arg1);//ok
		break;
		case TODO_TTS_STOP:
			err = AiuiCtrl_tts_stop();//ok
		break;
		case TODO_WAKTUP:
			err = AiuiCtrl_wakeup(true);//ok
		break;
		case TODO_SLEPP:
			err = AiuiCtrl_wakeup(false);//ok
		break;
		case TODO_GET_WIFI_STATUS:
			err = AiuiCtrl_get_wifi_status();
		break;
		case TODO_CONFIG_WIFI:
			err = AiuiCtrl_Config_wifi(ssid,passwd);
		break;
		case TODO_VOICE_OPEN:
			err = AiuiCtrl_Voice_EN(ENABLE);//ok
		break;
		case TODO_VOICE_STOP:
			err = AiuiCtrl_Voice_EN(DISABLE);//ok
		break;
		default:break;
	}
	return err;
}