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
#define RESULT			1  //结果事件
#define ERROR			2  //错误事件
#define STATE			3  //服务状态事件
		#define IDLE	1	//空闲
		#define	READY	2	//就绪，等待唤醒
		#define	WORKING	3	//已经唤醒，正在识别
#define	WAKEUP			4  	//唤醒事件。 info字段为唤醒结果 字段为唤醒结果字段为唤醒结果JSON字符串， 具体格式见 4.4.1. 4.4.1. 唤醒结果 唤醒结果 唤醒结果 。	
#define	SLEEP			5	//休眠事件。当出现交互超时，服务会进入状 休眠事件。
#define SLEEP_MAN		7	//手动进入睡眠
#define	VAD  			6	//VAD事件。当检测到输入音频的前端点后，会抛 事件。标识前后端点或者音量信息： 0（前端点）、 1（音量）、 2（后端点）。当 （后端点）。当 arg1 取 值为 1时，arg2为音量大小。
#define	VOICE_BEGIN_POINT	0
#define	VOICE_VALUE			1
#define	VOICE_END_POINT		2
		
#define	CMD_RETURN			8	//某条CMD 命令对应的返回事件。对于除CMD_GET_STATE外的有返回 命令，都会外的有返回 命令，用arg1标识对应的CMD命令， arg2 为 返回 值，0表示成功， 表示成功，info字段为描述信息。
//error code
#define MSP_ERROR_INVALID_PARA 			10106 //参数名称错误
#define MSP_ERROR_INVALID_PARA_VALUE 	10107 //参数取值错误		//
#define MSP_ERROR_NOT_FOUND 			10116 //云端无对应的scene场景参数		//
#define MSP_ERROR_NO_RESPONSE_DATA 		10120 //结果等待超时		//
#define MSP_ERROR_LMOD_RUNTIME_EXCEPTION 16005 //MSC 内部错误		//
#define ERROR_NO_NETWORK 				20001 //无有效的网络连接		//
#define ERROR_NETWORK_TIMEOUT 			20002 //网络连接超时		//
#define ERROR_NET_EXPECTION 			20003 //网络连接发生异常		//
#define ERROR_INVALID_RESULT 			20004 //无有效的结果		//
#define ERROR_NO_MATCH 					20005 //无匹配结果		//
#define ERROR_AUDIO_RECORD 				20006 //录音失败		//
#define ERROR_COMPONENT_NOT_INSTALLED 	21001 //没有安装服务组件		//
#define ERROR_SERVICE_BINDER_DIED 		21020 //与服务的绑定已消亡		//
#define ERROR_LOCAL_NO_INIT 			22001 //本地引擎未初始化		//
#define ERROR_UNKNOWN 					20999 //未知错误		//

//消息类型mesgtype
#define CMD_GET_STATE		1	//获取服务状态
#define CMD_RESET			4	//重置AIUI服务状态，服务会停止并重新启动，进入待唤醒状态
#define CMD_START			5	//启动服务
#define	CMD_STOP			6	//停止 AIUI服务。停止之后，将不服务。
#define CMD_RESET_WAKEUP	8   //重置唤醒状态。AIUI服务重置为待唤 醒状态。若当前为唤醒状态，发送该消 息重置后会抛出 EVENT_SLEEP事
#define CMD_SET_BEAM		9	//设置麦克风阵列的拾音波束。用arg1携带拾音波束号。
#define CMD_SET_PARAMS		10	//设置参数配。
#define	CMD_SEND_LOG		12	//发送应 用日志到云端，可以帮助分析发送应 
#define CMD_BUILD_GRAMMAR	16	//用 构建本地语法。用 params 字段携带 bnf语法
#define CMD_UPDATE_LOCAL_LEXICON	17	//更新 本地词表
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

#define RECV_BUF_LEN 		12
#define MSG_NORMAL_LEN 		4
#define MSG_EXTRA_LEN 		8
#define PACKET_LEN_BIT 		4
#define SYNC_HEAD_SECOND 	0x01


#define REQUSET		1
#define ACTION    	2
//获取状态
#define GET_WIFI_STATUS 	1
#define GET_TTS_STATUS  	2
#define GET_WAKEUP_STATUS  	3
#define GET_AIUI_STATUS		4
#define GET_LISTEN_STATUS	5

//请求处理命令
#define TODO_TTS_START			1
#define TODO_TTS_STOP			2
#define TODO_WAKTUP				3
#define	TODO_SLEPP				4
#define TODO_GET_WIFI_STATUS	5
#define TODO_CONFIG_WIFI		6
#define	TODO_VOICE_OPEN			7
#define	TODO_VOICE_STOP			8		
#define TODO_SMART_CNF_START	9
#define TODO_SMART_CNF_STOP		10
#define TODO_LOCAL_TTS			11
#define TODO_EKHO_TTS			12
#define TODO_ESPEAK_TTS			13

//发音人类型
#define XIAOFENG	"xiaofeng"
#define XIAOYAN		"xiaoyan"

#define ENABLE		1
#define DISABLE		0

struct aiui_status{
	unsigned int wifi_status;
	unsigned int tts_status;
	unsigned int wakeup_status;
	unsigned int aiui_status;
	unsigned int listen_status;
};

extern void process_recv(unsigned char* buf, int len);
extern int AiuiReset_wakeup(bool isResetWake);
extern int Aiui_config(char *appid,char *key,char *scene,bool launch);
extern int AiuiCtrl_tts_start(char *strbuff);
extern int AiuiCtrl_tts_stop();
extern int AiuiCtrl_get_wifi_status();
extern int AiuiCtrl_Voice_EN(int status);
extern int ask_aiui_todo(unsigned char type,const char *arg1,const char *arg2);
extern int get_aiui_status(unsigned char cmd);
//locat_tts.cpp
extern int local_tts_engine(char *name,char *text);

#ifdef __cplusplus
}
#endif

#endif
