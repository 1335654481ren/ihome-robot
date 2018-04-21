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

void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
    for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
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

/*
{
    "semantic": {
        "slots":    {
            "name": "张三"
        }
    },
    "rc":   0,
    "operation":    "CALL",
    "service":  "telephone",
    "text": "打电话给张三"
}


int json_create()
{
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
    cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));
    cJSON_AddItemToObject(root, "text", cJSON_CreateString("打电话给张三"));
    cJSON_AddItemToObject(root, "semantic", item);
    cJSON_AddItemToObject(item, "slots", next);
    cJSON_AddItemToObject(next, "name", cJSON_CreateString("张三"));

    printf("%s\n", cJSON_Print(root));

    return 0;
}
*/
void Event_result(cJSON *content)
{
/*
有格式的方式打印Json:
{
	"content":	{
		"result":	{
			"sid":	"cid6f1cb6e0@ch00d30bb03fd9010110",
			"intent":	{
				"operation":	"ANSWER",
				"sid":	"cid6f1cb6e0@ch00d30bb03fd9010110",
				"engine_time":	0.002000,
				"text":	"现在几点了",
				"service":	"datetime",
				"score":	1,
				"answer":	{
					"type":	"T",
					"text":	"现在是2016年12月17日 星期六 16:14:15"
				},
				"uuid":	"atn000e64ba@un7ed30bb03fd76f2a01",
				"cid":	"cid6f1cb6e0@ch00d30baf213d000000",
				"rc":	0,
				"match_info":	{
					"value":	"-----",
					"type":	"gparser_path"
				},
				"array_index":	0
			}
		},
		
"content":	{
	"result":	{
		"sid":	"cid6f1cb6e0@ch00d30bb04667010114",
		"intent":	{
			"sid":	"cid6f1cb6e0@ch00d30bb04667010114",
			"text": "向左转",
			"uuid": "atn000e6939@un7ed30bb046656f2a01",
			"cid":	"cid6f1cb6e0@ch00d30baf213d000000",
			"rc":	4
		}
	},
	"arg2": 0,
	"eventType":	1,

*/
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
					//AiuiCtrl_tts_start("主人，我已经收到你的命令了哦");
				}
			}
		}
	}
}
void Event_error(cJSON *content)
{

}
void Event_state(cJSON *content)
{
	char AIUI_STate[][10]={"idle","ready","working"};
/*
有格式的方式打印Json:
{
	"content":	{
		"arg1":	3,
		"arg2":	0,
		"eventType":	3
	},
	"type":	"aiui_event"
}
*/
	int arg1,arg2;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(content, "arg2");		
	arg2 = atoi(cJSON_Print(item));
//	printf("Aiui state:%s %d\n",AIUI_STate[arg1-1],arg2);
	switch(arg1){
		case IDLE:
			break;
		case READY:
			break;
		case WORKING:
			break;
		default:break;
	}
}
void Event_wakeup(cJSON *content)
{
/*
有格式的方式打印Json:
{
	"content":	{
		"arg1":	0,
		"info":	{
			"power":	345958907904,
			"beam":	3,
			"CMScore":	109,
			"channel":	2,
			"angle":	135
		},
		"arg2":	0,
		"eventType":	4
	},
	"type":	"aiui_event"
}
*/
	int arg1,arg2,channel,angle;
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
	printf("arg1:%d arg2:%d \nchannel:%d  angle:%d\n",arg1,arg2,channel,angle);
}
void Event_sleep(cJSON *object)
{

}
void Event_vad(cJSON *content)
{
/*
有格式的方式打印Json:
{
	"content":	{
		"arg1":	0,//0:前端点，2:后端点，1:音量
		"arg2":	0,//当arg1=1，arg2=音量大小
		"eventType":	6
	},
	"type":	"aiui_event"
}
*/
	int arg1,arg2;
	cJSON * item = NULL;//cjson对象
	item = cJSON_GetObjectItem(content, "arg1");		
	arg1 = atoi(cJSON_Print(item));
	item = cJSON_GetObjectItem(content, "arg2");		
	arg2 = atoi(cJSON_Print(item));
	switch(arg1){
		case VOICE_BEGIN_POINT:
//			printf("check get the voice begin\n");
			break;
		case VOICE_VALUE:
//			printf("Now the Aiui voice value:%d\n",arg2);
			break;
		case VOICE_END_POINT:
//			printf("check get the voice end\n");
			break;
		default:break;
	}
}
void Event_return(cJSON *content)
{

}

void AIUI_event_deal(int eventType,cJSON *content)
{
	switch(eventType)
	{
		case RESULT:
			Event_result(content);
			break;
		case ERROR:
			break;
		case STATE:
			Event_state(content);
			break;
		case WAKEUP:
			Event_wakeup(content);
			break;
		case SLEEP:
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
 //       printf("Error before: error[%s]\n",cJSON_GetErrorPtr());
		return -1;
    }
    else
    {
       printf("%s\n", "有格式的方式打印Json:");           
       printf("%s\n\n", cJSON_Print(pRoot));
 //     printf("%s\n", "获取content下的cjson对象:");
 		pContent = cJSON_GetObjectItem(pRoot, "content");//
 		pType = cJSON_GetObjectItem(pRoot, "type");//
		//wifi_status处理
		if ((NULL != pType) && (0 == strcmp(pType->valuestring, "aiui_event")))
		{
//			 printf("aiui_event处理:\n");
			 item = cJSON_GetObjectItem(pContent, "eventType");		
			 eventType = atoi(cJSON_Print(item));
//			 printf("EventType::%d\n",eventType);
			 AIUI_event_deal(eventType,pContent);
		}
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "wifi_status")))
		{
//			 printf("wifi_status处理:\n");
			 item = cJSON_GetObjectItem(pContent, "ssid");		
//			 printf("ssid = %s\n",cJSON_Print(item));
			 item = cJSON_GetObjectItem(pContent, "connected");		

			 if(item->valueint == cJSON_False)
			 {
//			 	printf("wifi connected error!!!!\n");
			 }
			 else if(item->valueint == cJSON_True)
			 {
//			 	printf("wifi connected ok<^>\n");
			 }
		}
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "tts_event")))
		{
//			 printf("tts_event处理:\n");
			 cJSON* pEventType = cJSON_GetObjectItem(pContent, "eventType");
//			 printf("pEventType->valueint:%d\n", pEventType->valueint);
			 if ((NULL != pEventType) && (pEventType->valueint == 1))
			 {
//			 	printf("tts ok!\n");
			 }
		}
    }
	cJSON_Delete(pRoot);
    return 0;    
}

void AIUI_cJSON_Parse(cJSON* pJson)
{
	if (NULL != pJson)
	{
		printf("parse json successful\n");
		cJSON* pType = cJSON_GetObjectItem(pJson, "type");
		//aiui_event处理
		if ((NULL != pType) && (0 == strcmp(pType->valuestring, "aiui_event")))
		{
			printf("type:%s\n", pType->valuestring);
			cJSON* pContent = cJSON_GetObjectItem(pJson, "content");
			if (NULL != pContent)
			{
				cJSON* pArg1 = cJSON_GetObjectItem(pContent, "arg1");
				cJSON* pArg2 = cJSON_GetObjectItem(pContent, "arg2");
				cJSON* pEventType = cJSON_GetObjectItem(pContent, "eventType");
				if ((NULL != pEventType) && (NULL != pArg1) && (NULL != pArg2))
				{
					printf("eventType:%d\n", pEventType->valueint);
					//EVENT_RESULT
					if (1 == pEventType->valueint)
					{		
						//data字段为result数据
						//info字段为描述数据的json
						printf("EVENT_RESULT\n");
						cJSON* pInfo = cJSON_GetObjectItem(pContent, "info");
						cJSON* pSub = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(pInfo, "data")->child, "params"), "sub");
						if ((NULL != pSub) && 0 == (strcmp(pSub->valuestring, "nlp")))
						{
							cJSON* pText = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(pContent, "result"), "intent"), "text");
							if (NULL != pText)
							{						
								printf("result:%s\n", pText->valuestring);
							}
						}
					}
					//EVENT_ERROR
					else if (2 == pEventType->valueint)
					{
						cJSON* pInfo = cJSON_GetObjectItem(pContent, "info");
						//arg1字段为错误码
						//info字段为错误描述信息
						printf("EVENT_ERROR\n");
						if (10106 == pArg1->valueint)
						{
							printf("MSP_ERROR_INVALID_PARA 10106 参数名称错误\n");
						}
						else if (10107 == pArg1->valueint)
						{
							printf("MSP_ERROR_INVALID_PARA_VALUE 10107 参数取值错误\n");
						}
						else if (10116 == pArg1->valueint)
						{
							printf("MSP_ERROR_NOT_FOUND 10116 云端无对应的scene场景参数\n");
						}
						else if (10120 == pArg1->valueint)
						{
							printf("MSP_ERROR_NO_RESPONSE_DATA 10120 结果等待超时\n");
						}
						else if (16005 == pArg1->valueint)
						{
							printf("MSP_ERROR_LMOD_RUNTIME_EXCEPTION 16005 MSC 内部错误\n");
						}
						else if (20001 == pArg1->valueint)
						{
							printf("ERROR_NO_NETWORK 20001 无有效的网络连接\n");
						}
						else if (20002 == pInfo->valueint)
						{
							printf("ERROR_NETWORK_TIMEOUT 20002 网络连接超时\n");
						}
						else if (20003 == pInfo->valueint)
						{
							printf("ERROR_NET_EXPECTION 20003 网络连接发生异常\n");
						}
						else if (20004 == pInfo->valueint)
						{
							printf("ERROR_INVALID_RESULT 20004 无有效的结果\n");	
						}
						else if (20005 == pInfo->valueint)
						{
							printf("ERROR_NO_MATCH 20005 无匹配结果\n");
						}
						else if (20006 == pInfo->valueint)
						{
							printf("ERROR_AUDIO_RECORD 20006 录音失败\n");
						}
						else if (21001 == pInfo->valueint)
						{
							printf("ERROR_COMPONENT_NOT_INSTALLED 21001 没有安装服务组件\n");
						}
						else if (21020 == pInfo->valueint)
						{
							printf("ERROR_SERVICE_BINDER_DIED 21020 与服务的绑定已消亡\n");
						}
						else if (22001 == pInfo->valueint)
						{
							printf("ERROR_LOCAL_NO_INIT 22001 本地引擎未初始化\n");
						}
						else if (20999 == pInfo->valueint)
						{
							printf("ERROR_UNKNOWN 20999 未知错误\n");
						}
					}
					//EVENT_STATE
					else if (3 == pEventType->valueint)
					{
						//arg1字段
						//STATE_IDLE(空闲状态)
						//STATE_READY(就绪状态)
						//STATE_WORKING(工作状态)
						printf("EVENT_STATE\n");
						printf("arg1:%d\n", pArg1->valueint);
					}
					//EVENT_WAKEUP
					else if (4 == pEventType->valueint)
					{

						//info字段为唤醒结果json
						//"power":12342435436	唤醒能量值
						//"beam":3				拾音波束号,唤醒成功后阵列将在该波束方向上拾音
						//"angle":180			唤醒角度
						//"channel":5			唤醒声道,即麦克风编号,表示该声道的音频质量最好
						//"CMScore":132			声道对应的唤醒得分
						printf("EVENT_WAKEUP\n");
						cJSON* pInfo = cJSON_GetObjectItem(pContent, "info");
						printf("power:%d\n", cJSON_GetObjectItem(pInfo, "power")->valueint);
						printf("beam:%d\n", cJSON_GetObjectItem(pInfo, "beam")->valueint);
						printf("angle:%d\n", cJSON_GetObjectItem(pInfo, "angle")->valueint);
						printf("channel:%d\n", cJSON_GetObjectItem(pInfo, "channel")->valueint);
						printf("CMScore:%d\n", cJSON_GetObjectItem(pInfo, "CMScore")->valueint);
					}
					//EVENT_SLEEP
					else if (5 == pEventType->valueint)
					{
						printf("EVENT_SLEEP\n");
						//手动唤醒AIUI
					}
					//EVENT_VAD
					else if (6 == pEventType->valueint)
					{
						//arg1标识前后端点或者音量信息
						//0(前端点) 1(音量) 2(后端点)
						//当arg1=1时,arg2为音量大小	
						printf("EVENT_VAD\n");
						//if (1 == pArg1->valueint)
						//{
							printf("arg1:%d arg2:%d\n", pArg1->valueint, pArg2->valueint);
						//}
					}
				}
			}
		}
		//wifi_status处理
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "wifi_status")))
		{
			printf("wifi_status处理\n");
		}
		else if ((NULL != pType) && (0 == strcmp(pType->valuestring, "tts_event")))
		{
			printf("tts_event处理\n");
			cJSON* pContent = cJSON_GetObjectItem(pJson, "content");
			if (NULL != pContent)
			{
				cJSON* pEventType = cJSON_GetObjectItem(pContent, "eventType");
				printf("pEventType->valueint:%d\n", pEventType->valueint);
				if ((NULL != pEventType) && (pEventType->valueint == 1))
				{
				}
			}
		}
	}
}
 
// gzip格式
// (u16)MAGIC + (u8)DEFLATE + (u8)flag + (u32)time + (u8)deflate_flag + (u8)OS_CODE

int data_to_json(unsigned char *buff, int len)
{
    unsigned long read_len = 8196;
    unsigned char read_buf[8196];

    if(gzuncompress(buff, len, read_buf, &read_len)== -1)
		return -1;
//	printf("get json:\n");
//	printf("%s\n",read_buf);
	if(json_pasre((char *)read_buf)== -1)
		return -1;
/*   
	pid_t status;
    int wfd = 0; 
    int rfd = 0;

    wfd = open("Data.gz", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (wfd < 0)
    {
        perror("\n");
        return -1;
    }
    write(wfd,buff,len);
    close(wfd);

     status = system("gunzip Data.gz");
	if (-1 == status)	
	{	
	printf("system error!");  
	}	
	else  
	{	
	//	printf("exit status value = [0x%x]\n", status);  

		if (WIFEXITED(status))  
		{	
			if (0 == WEXITSTATUS(status))	
			{	
			//	 printf("run shell script successfully.\n");  
				 
				 rfd = open("Data", O_RDONLY);
				 if (-1 == rfd)
				 {
					 perror("\n");
					 return -1;
				 }
				 if((read_len = read(rfd, read_buf, READ_LEN)) >= 0)
				 {
				 //	printf("get json:\n");
				 //	printf("%s\n",read_buf);
					json_pasre(read_buf);
				 }
				 close(rfd);
				 system("rm Data -rf");
			}	
			else  
			{	
			 	printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));	
			}	
		}	
		else  
		{	
		printf("exit status = [%d]\n", WEXITSTATUS(status));  
		}	
	}
*/
    return 0;
}
