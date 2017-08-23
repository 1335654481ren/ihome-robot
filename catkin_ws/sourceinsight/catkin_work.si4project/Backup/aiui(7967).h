/*

*/

#ifndef AIUI__h
#define AIUI__h

#ifdef __cplusplus
extern "C"
{
#endif

//define AIUI config type
#define 	AIUI_CFG		"aiui_cfg"
//define AIUI contrl event msg type
#define		AIUI_EVENT		"aiui_event"
#define		TTS_EVENT		"tts_event"
#define		WIFI_STATUS		"wifi_status"		
//定义AIUI消息数据AIUI_EVENT;
#define RESULT		1  //结果事件
#define ERROR		2  //错误事件
#define STATE		3  //服务状态事件
#define IDLE	0	//空闲
#define	READY	1	//就绪，等待唤醒
#define	WORKING	2	//已经唤醒，正在识别
#define	WAKEUP		4  	//唤醒事件。 唤醒事件。 info字段为唤醒结果 字段为唤醒结果字段为唤醒结果 字段为唤醒结果 字段为唤醒结果 JSON JSONJSON字符串， 字符串， 具体格式见 具体格式见 具体格式见 4.4.1. 4.4.1. 唤醒结果 唤醒结果 唤醒结果 。	
#define	SLEEP		5	//休眠事件。当出现交互超时，服务会进入状 休眠事件。当出现交互超时，服务会进入状 休眠事件。当出现交互超时，服务会进入状
#define	VAD  		6	//VAD事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 事件。当检测到输入音频的前端点后，会抛 出该事件，用 出该事件，用 出该事件，用 arg1 arg1 标识前后端点或者音量信息： 标识前后端点或者音量信息： 标识前后端点或者音量信息： 标识前后端点或者音量信息： 标识前后端点或者音量信息： 标识前后端点或者音量信息： 标识前后端点或者音量信息： 0（前端点）、 （前端点）、 （前端点）、 （前端点）、 1（音量）、 （音量）、 （音量）、 2（后端点）。当 （后端点）。当 （后端点）。当 （后端点）。当 （后端点）。当 arg1 arg1 取 值为 1时， arg2 arg2 为音量大小。 为音量大小。 为音量大小。 为
#define	VOICE_BEGIN_POINT	0
#define	VOICE_VALUE			1
#define	VOICE_END_POINT	2
		
#define	CMD_RETURN	8	//某条CMD 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 命令对应的返回事件。 对于除 对于除 对于除CMD_GET_STATE外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会外的有返回 命令，都会该事件， 该事件， 用 arg1 arg1 标识对应的 标识对应的 标识对应的 CMDCMDCMD命令， 命令， arg2 arg2 为 返回 值，0表示成功， 表示成功， 表示成功， infoinfo info字段为描述信息。 字段为描述信息。 字段为描述信息。 字

//消息类型mesgtype
#define CMD_GET_STATE		1	//获取服务状态
#define CMD_RESET			4	//重置AIUI服务状态，服务会停止并重新启动，进入待唤醒状态
#define CMD_START			5	//启动服务
#define	CMD_STOP			6	//停止 AIUI服务。停止之后，将不服务。停止之后，将不 服务。停止之后，将不 服务。停止之后，将不 服务。停止之后，将不 响应外部输入。 响应外部输入。 响
#define CMD_RESET_WAKEUP	8   //重置唤醒状态。 重置唤醒状态。 重置唤醒状态。 AIUIAIUIAIUIAIUI服务重置为待唤 服务重置为待唤 服务重置为待唤 醒状态。 醒状态。 若当前为唤醒状态，发送该消 若当前为唤醒状态，发送该消 若当前为唤醒状态，发送该消 若当前为唤醒状态，发送该消 若当前为唤醒状态，发送该消 若当前为唤醒状态，发送该消 息重置后会抛出 息重置后会抛出 息重置后会抛出 EVENT_SLEEPEVENT_SLEEPEVENT_SLEEPEVENT_SLEEPEVENT_SLEEP EVENT_SLEEPEVENT_SLEEP EVENT_SLEEP事
#define CMD_SET_BEAM		9	//设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 设置麦克风阵列的拾音波束。 用 arg1 arg1 携带拾音波束号。 携带拾音波束号。 携带拾音波束号。
#define CMD_SET_PARAMS		10	//设置参数配。
#define	CMD_SEND_LOG		12	//发送应 用日志到云端，可以帮助分析发送应 用日志到云端，可以帮助分析发送应 用日志到云端，可以帮助分析发送应 用日志到云端，可以帮助分析
#define CMD_BUILD_GRAMMAR	16	//构建本地语法。用 构建本地语法。用 构建本地语法。用 构建本地语法。用 params params params字段携带 字段携带 bnfbnf 语法内容， 语法内容，
#define CMD_UPDATE_LOCAL_LEXICON	17	//更新 本地词表 本地词表 本地词表 本地词表
//消息类型
#define SYNC_HEAD 	0xA5
#define USER_ID 	0x01

#define SHAKE_HAND_TYPE 		0x01
#define WIFI_CONFIG_TYPE 		0x02
#define AIUI_CONFIG_TYPE 		0x03
#define AIUI_MESSAGE_TYPE 		0x04
#define CONTROL_MESSAGE_TYPE 	0x05
#define CUSTOM_DATA_TYPE 		0x2A
#define CONFIRM_MESSAGE_TYPE 	0xff
//数据帧解析定义
#define AIUI_FRAM_HEAD_COST		7
#define AIUI_FRAM_COST			8

#define RECV_BUF_LEN 12
#define MSG_NORMAL_LEN 4
#define MSG_EXTRA_LEN 8
#define PACKET_LEN_BIT 4
#define SYNC_HEAD_SECOND 0x01

#define REQUSET		1
#define ACTION    	2

#define GET_WIFI_STATUS 	1
#define GET_TTS_STATUS  	2
#define GET_WAKEUP_STATUS  	3

#define TODO_TTS_START			1
#define TODO_TTS_STOP			2
#define TODO_WAKTUP				3
#define	TODO_SLEPP				4
#define TODO_GET_WIFI_STATUS	5
#define TODO_CONFIG_WIFI		6
#define	TODO_VOICE_OPEN			7
#define	TODO_VOICE_STOP			8		

#define ENABLE		1
#define DISABLE		0

extern void process_recv(unsigned char* buf, int len);
extern int AiuiReset_wakeup(bool isResetWake);
extern int Aiui_config(char *appid,char *key,char *scene,bool launch);
extern int AiuiCtrl_tts_start(char *strbuff);
extern int AiuiCtrl_tts_stop();
extern int AiuiCtrl_get_wifi_status();
extern int AiuiCtrl_Voice_EN(int status);
extern int ask_aiui_todo(unsigned char type,const char *arg1,const char *arg2);
#ifdef __cplusplus
}
#endif

#endif
