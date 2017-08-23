#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <WeChatAPI_C.h>

#define WECHAT_CLOUD_SERVICE 1
void onWeChatCallBack(int, int, unsigned int, const unsigned char*, unsigned int)
{
	printf("onWeChatCallBack\n");
}
void onReceVNotify(unsigned int, const unsigned char*, unsigned int)
{
	printf("onReceVNotify\n");
}
void onHandleEvent(EventValue event)
{
	printf("onHandleEvent\n");
}

int main(int argc, char** agrv)
{
	int taskid = 0;
	char *devlicence = "renxiaoliang";
	char state[100] = "1234567dfghjk";
	printf("Try to start plugins\n");
	if( false == WeChatAPI_start(devlicence,strlen(devlicence)))
	{
		printf("Start SDK failed check licence\r\n");
		return -1;
	}
	WeChatAPI_setCallBack(onWeChatCallBack);
	WeChatAPI_setNotifyCallBack(onReceVNotify);
	WeChatAPI_setSDKEventCallBack(onHandleEvent);
	printf("SDK Version:%s\n",WeChatAPI_getSDKVersion());
	printf("VenderID:%s\n", WeChatAPI_getVenderId());
	printf("DeviceID:%s\n", WeChatAPI_getDeviceId());
	sleep(1);
	taskid = WeChatAPI_sendDataToServer(WECHAT_CLOUD_SERVICE,(const unsigned char *)state,strlen(state));
	printf("task id is%d",taskid);
	return 0;
}
