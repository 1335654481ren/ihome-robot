#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zconf.h>
#include <zlib.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include "gzlib.h"
#include "cJSON.h"
#include "aiui.h"

#include <ros/ros.h>
#include <ros/time.h>
#include <robot_msgs/Ear.h>

extern ros::Publisher pub_ear;

struct aiui_status aiui;

void pub_ear_string(char *data)
{
	robot_msgs::Ear msg;
	msg.header.seq = 0;
	msg.header.stamp = ros::Time::now();
	msg.header.frame_id = "world";
	msg.id = 1;
	msg.type = 2;
	std::stringstream ss;
	ss << data;
	msg.data = ss.str();

	pub_ear.publish(msg);   //发布信息msg到每一个相连接的节点
}

void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
    for(int i = 0; i < cJSON_GetArraySize(root); i++ )   //遍历最外层json键值对
    {
        cJSON * item = cJSON_GetArrayItem(root, i);        
        if(cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
            printJson(item);
        else                                //值不为json对象就直接打印出键和值
        {
            printf("%s->", item->string);
            printf("%s\n", cJSON_Print(item));
        }
    }
}

void Event_result(cJSON *content)
{
	cJSON * result = NULL;//cjson对象
	cJSON * intent = NULL;//cjson对象
	cJSON * text = NULL;//cjson对象
	cJSON * answer = NULL;//cjson对象
	if((result = cJSON_GetObjectItem(content, "result"))!= NULL)
	{
		if((intent = cJSON_GetObjectItem(result, "intent"))!= NULL)
		{
			if((text = cJSON_GetObjectItem(intent, "text"))!= NULL)
			{
				printf("recognize words:%s\n",cJSON_Print(text));
				if((answer = cJSON_GetObjectItem(intent, "answer"))!= NULL)
				{
					text = cJSON_GetObjectItem(answer, "text");
					printf("Answer words:%s\n",cJSON_Print(text));
				}
				else
				{
					pub_ear_string(cJSON_Print(text));
				}
			}
		}
	}
}

void Event_error(cJSON *content)
{
	int arg1;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	printf("Event error code %d\n",arg1);
	switch(arg1){
		case ERROR_NET_EXPECTION:
			break;
		default:break;
	}
}

void Event_state(cJSON *content)
{
	char AIUI_STate[][10]={"idle","ready","working"};

	int arg1;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));

	printf("Aiui state:%s %d\n",AIUI_STate[arg1-1],arg1);
	switch(arg1){
		case IDLE:
			aiui.aiui_status = IDLE;
			break;
		case READY:
			aiui.aiui_status = READY;
			break;
		case WORKING:
			aiui.aiui_status = WORKING;
			break;
		default:break;
	}
}

void Event_wakeup(cJSON *content)
{
	int arg1,arg2,channel,angle,beam,CMScore;
	cJSON * item = NULL;//cjson对象
	cJSON * child = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(content, "arg2");		
	arg2 = atoi(cJSON_Print(item));
	child = cJSON_GetObjectItem(content, "info");
	item = cJSON_GetObjectItem(child, "channel");
	channel = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(child, "angle");
	angle = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(child, "beam");
	beam = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(child, "CMScore");
	CMScore = atoi(cJSON_Print(item));
	aiui.wakeup_status = WAKEUP;
	printf("arg1:%d arg2:%d \nchannel:%d  angle:%d\n",arg1,arg2,channel,angle);
}

void Event_sleep(cJSON *content)
{
	int arg1;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	switch(arg1){
		case 0:
			aiui.wakeup_status = SLEEP;
			printf("auto to sleep = %d\n",arg1 );
			break;
		case 1:
			aiui.wakeup_status = SLEEP_MAN;
			printf("manual to sleep =%d\n",arg1);
			break;

		default:break;
	}
}

void Event_vad(cJSON *content)
{
	int arg1,arg2;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(content, "arg2");		
	arg2 = atoi(cJSON_Print(item));
	switch(arg1){
		case VOICE_BEGIN_POINT:
			printf("check get the voice begin\n");
			break;
		case VOICE_VALUE:
			printf("Now the Aiui voice value:%d\n",arg2);
			break;
		case VOICE_END_POINT:
			printf("check get the voice end\n");
			break;
		default:break;
	}
}

void AIUI_event_deal(int eventType,cJSON *content)
{
	switch(eventType)
	{
		case RESULT:
			Event_result(content);
			break;
		case ERROR:
			Event_error(content);
			break;
		case STATE:
			Event_state(content);
			break;
		case WAKEUP:
			Event_wakeup(content);
			break;
		case SLEEP:
			Event_sleep(content);
			break;
		case VAD:
			Event_vad(content);
			break;
		case CMD_RETURN:
			break;
		default:break;	
	}
}

int json_pasre(char * jsonStr)
{
    cJSON * pRoot = NULL;
    cJSON * item = NULL;//cjson对象
    cJSON *pContent = NULL;
    cJSON *pType = NULL;//cjson对象
	int eventType = 0;
	pRoot = cJSON_Parse(jsonStr);
	
    if (!pRoot) 
    {
        printf("Error before: error[%s]\n",cJSON_GetErrorPtr());
		return -1;
    }
    else
    {
 //     printf("%s\n", "有格式的方式打印Json:");           
//      printf("%s\n\n", cJSON_Print(pRoot));
 		pContent = cJSON_GetObjectItem(pRoot, "content");//
 		pType = cJSON_GetObjectItem(pRoot, "type");//
		if ((NULL != pType) && (0 == strcmp(pType->valuestring, "aiui_event")))
		{
			 item = cJSON_GetObjectItem(pContent, "eventType");		
			 eventType = atoi(cJSON_Print(item));
			 AIUI_event_deal(eventType,pContent);
		}
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "wifi_status")))//wifi_status处理
		{
			 printf("wifi_status处理:\n");
			 item = cJSON_GetObjectItem(pContent, "ssid");		
			 printf("ssid = %s\n",cJSON_Print(item));
			 item = cJSON_GetObjectItem(pContent, "connected");		

			 if(item->valueint == cJSON_False)
			 {
			 	printf("wifi connected error!!!!\n");
				aiui.wifi_status = 0;
			 }
			 else if(item->valueint == cJSON_True)
			 {
			 	printf("wifi connected ok<^>\n");
				aiui.wifi_status = 1;
			 }
		}
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "tts_event")))//tts reuslt
		{
			 cJSON* pEventType = cJSON_GetObjectItem(pContent, "eventType");
			 if ((NULL != pEventType) && (pEventType->valueint == 1))
			 {
			 	printf("tts ok!\n");
				aiui.tts_status = 0;
			 }
		}
    }
	cJSON_Delete(pRoot);
    return 0;    
}
// gzip格式
// (u16)MAGIC + (u8)DEFLATE + (u8)flag + (u32)time + (u8)deflate_flag + (u8)OS_CODE

int data_to_json(unsigned char *buff, int len)
{
    unsigned long read_len = 8196;
    unsigned char read_buf[8196];

    if(gzuncompress(buff, len, read_buf, &read_len)== -1)
		return -1;
	
	if(json_pasre((char *)read_buf)== -1)
		return -1;
    return 0;
}

int get_aiui_status(unsigned char cmd)
{
	unsigned int err = 0;
	switch(cmd)
	{
		case GET_WIFI_STATUS:
			err = aiui.wifi_status;
			break;
		case GET_TTS_STATUS:
			err = aiui.tts_status;
			break;
		case GET_WAKEUP_STATUS:
			err = aiui.wakeup_status;
			break;
		case GET_AIUI_STATUS:
			err = aiui.aiui_status;
			break;
		case GET_LISTEN_STATUS:
			err = aiui.listen_status;
			break;
		default: err = 0;
			break;
	}
	return err;
}
