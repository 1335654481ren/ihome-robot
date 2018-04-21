/*
 * WeChatAPI_C.h
 *
 *  Created on: Feb 22, 2016
 *      Author: zorrowu
 */

#ifndef SDKAPI_WECHATAPI_C_H_
#define SDKAPI_WECHATAPI_C_H_

#include <stdint.h>
#include <stdbool.h>

enum EventValue
{
	EVENT_VALUE_LOGIN 	= 1,
	EVENT_VALUE_LOGOUT 	= 2
};

typedef void (*onReceiveResponse) (int taskid, int errcode, unsigned int funcid, const unsigned char* body, unsigned int bodylen);
typedef void (*onReceiveNotify) (unsigned int funcid, const unsigned char* body, unsigned int bodylen);
typedef void (*onSDKEventCallback) (enum EventValue event_value);

bool WeChatAPI_start(const char* devlicence, unsigned int licencelen);
int WeChatAPI_sendDataToServer(unsigned int funcid, const unsigned char* body, unsigned int bodylen);
void WeChatAPI_setCallBack(onReceiveResponse _callback);
void WeChatAPI_setNotifyCallBack(onReceiveNotify _callback);
void WeChatAPI_setSDKEventCallBack(onSDKEventCallback _callback);
int WeChatAPI_checkUpdate(const unsigned char* body, unsigned int bodylen);
const char* WeChatAPI_getVenderId();
const char* WeChatAPI_getDeviceId();
const char* WeChatAPI_getSDKVersion();
void WeChatAPI_release();

#endif /* SDKAPI_WECHATAPI_C_H_ */
