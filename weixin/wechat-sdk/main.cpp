#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include <WeChatAPI.h>

#define WECHAT_CLOUD_SERVICE 1

using namespace std;
/*{
"base_resp":{"errcode":0,"errmsg":"ok"},
"deviceid":"gh_42fceb1b30b9_72fd75228f4b5ba1",
"qrticket":"http:\/\/we.qq.com\/d\/AQBTQaHVdUy_bqX3ytV-gimOElSViS6A-ssECtfG",
"devicelicence":"FB8250F47A04BBBD9F0C2D871404A8B99BDF534DB30BAF6EE3445EB129A63527333D23F47ADF86174BC4A05F0BCF0EDA9F344D081700669C8741F81FA9331D41F83DC61C250D6A3314D5EBA4FA8AE536"}
*/
extern "C"{

int taskid = 0;
string devicelicence = "FB8250F47A04BBBD9F0C2D871404A8B99BDF534DB30BAF6EE3445EB129A63527333D23F47ADF86174BC4A05F0BCF0EDA9F344D081700669C8741F81FA9331D41F83DC61C250D6A3314D5EBA4FA8AE536";
char state[100] = "1234567dfghjk";

void onWeChatCallBack(int, int, unsigned int, const unsigned char*, unsigned int)
{
	printf("onWeChatCallBack\n");
}
void onRecevNotify(unsigned int, const unsigned char*, unsigned int)
{
	printf("onReceVNotify\n");
}
void onHandleEvent(EventValue event)
{
	printf("onHandleEvent\n");
}

}

int main(int argc, char** agrv)
{
	std::cout << "Try to start plugins!" <<std::endl;
	bool flag = WeChatAPI::instance()->start(devicelicence.c_str(),devicelicence.size());
	if( flag == false)
	{
		std::cout << "Start SDK failed check licence!!" << std::endl;
		return -1;
	}	
	WeChatAPI::instance()->setCallBack(onWeChatCallBack);
	WeChatAPI::instance()->setNotifyCallBack(onRecevNotify);
	WeChatAPI::instance()->setSDKEventCallBack(onHandleEvent);
	std::cout << "VenderID:" << WeChatAPI::instance()->getVenderId() << std::endl;
	std::cout << "DeviceID:" <<WeChatAPI::instance()->getDeviceId() << std::endl;
	sleep(1);
	WeChatAPI::instance()->sendDataToServer(WECHAT_CLOUD_SERVICE,(const unsigned char *)state,strlen(state));
	std::cout << "task id is " << taskid << std::endl;

	return 0;
}
