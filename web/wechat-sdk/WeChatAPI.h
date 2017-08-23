/*
 * WeChatAPI.h
 *
 *  Created on: 2015-10-29
 *      Author: zorrowu
 */

#ifndef WECHATAPI_H_
#define WECHATAPI_H_
#include <string>

enum EventValue
{
	EVENT_VALUE_LOGIN 	= 1,
	EVENT_VALUE_LOGOUT 	= 2
};

typedef void (*onReceiveResponse) (int taskid, int errcode, unsigned int funcid, const unsigned char* body, unsigned int bodylen);
typedef void (*onReceiveNotify) (unsigned int funcid, const unsigned char* body, unsigned int bodylen);
typedef void (*onSDKEventCallback) (EventValue event_value);



class WeChatAPI {
public:
	static WeChatAPI* instance();
	static void release();

private:
	WeChatAPI();
	virtual ~WeChatAPI();
	void __onReceiveNotify(unsigned int funcid, const unsigned char* body, unsigned int bodylen);
	void __onReceiveResponse(int taskid, int errcode, unsigned int funcid, const unsigned char* body, unsigned int bodylen);
	void __onReceiveEvent(EventValue event_value);

public:
	bool start(const char* devlicence, unsigned int licencelen);
	int sendDataToServer(unsigned int funcid, const unsigned char* body, unsigned int bodylen);
	void setCallBack(onReceiveResponse _callback);
	void setNotifyCallBack(onReceiveNotify _callback);
	void setSDKEventCallBack(onSDKEventCallback _callback);
	int checkUpdate(const unsigned char* body, unsigned int bodylen);
	const char* getVenderId();
	const char* getDeviceId();
	const char* getSDKVersion();


private:
	static WeChatAPI* s_instance;
	onReceiveResponse m_callback;
	onReceiveNotify m_notify;
	onSDKEventCallback m_event;
};

#endif /* WECHATAPI_H_ */
