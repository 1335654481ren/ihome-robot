/*
* 语音合成（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的
* 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的
* 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/soundcard.h>
#include <alsa/asoundlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

using namespace std;

#define SERVER_PORT 12345
#define BUFF_LEN 16*1024

//#define Audio_Device "/dev/dsp"

 //不同的声卡有着不同的播放参数，这些参数可以使用file命令获得
#define Sample_Size 16 //there're two kinds of bits,8 bits and 16 bits

#define Sample_Rate 16000 //sampling rate

typedef int SR_DWORD;
typedef short int SR_WORD ;

/* wav音频头部格式 */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = 下一个结构体的大小 : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = 通道数 : 1
	int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
	short int       bits_per_sample;        // = 量化比特数: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = 纯数据长度 : FileSize - 44 
} wave_pcm_hdr;

/* 默认wav音频头部数据 */
wave_pcm_hdr default_wav_hdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};



/* 文本合成 */
int text_to_speech(const char* src_text, const char* params,wave_pcm_hdr* wav_hdr,char** synth_speech)
{
	int          ret          = -1;
	const char*  sessionID    = NULL;
	unsigned int audio_len    = 0;
	
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
	unsigned int    speech_len = 0;
	*wav_hdr      = default_wav_hdr;

	if (NULL == src_text)
	{
		printf("params is error!\n");
		return ret;
	}

	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		
		return ret;
	}
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
    pthread_mutex_unlock(&mutex);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n",ret);
                printf("src_text=%s\n",src_text);
		QTTSSessionEnd(sessionID, "TextPutError");
		
		return ret;
	}

        *synth_speech = (char*)malloc(2 * 1024 * 1024);
	while (1) 
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
                //printf("audio_len=%d\n",audio_len);
                if (MSP_SUCCESS != ret)
			break;
		if (NULL != data && 0 != audio_len)
		{
		    wav_hdr->data_size += audio_len; //计算data_size大小
		    
		    memset( *synth_speech + speech_len, 0, audio_len );
		    memcpy( *synth_speech + speech_len, data, audio_len );
		    speech_len += audio_len;
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}
	

	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n",ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		//fclose(fp);
		//return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr->size_8 += wav_hdr->data_size + (sizeof(wav_hdr) - 8);
	
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n",ret);
	}

	return ret;
}


/*播放器*/
int set_pcm_play(wave_pcm_hdr* wav_hdr,char* synth_speech)
{
        int rc;
        int ret;
        int size;

        snd_pcm_t* handle; //PCI设备句柄
        snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
        unsigned int val;
        int dir=0;
        snd_pcm_uframes_t frames,periodsize;
        char *buffer;
        int channels=wav_hdr->channels;
        int frequency=wav_hdr->samples_per_sec;
        int bit=wav_hdr->bits_per_sample;
        int datablock=wav_hdr->block_align;
        
        rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if(rc<0)
        {
                perror("\nopen PCM device failed:");
                return -1;
        }


        snd_pcm_hw_params_alloca(&params); //分配params结构体
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_alloca:");
                return -1;
        }
         rc=snd_pcm_hw_params_any(handle, params);//初始化params
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_any:");
                return -1;
        }
        rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
        if(rc<0)
        {
                perror("\nsed_pcm_hw_set_access:");
                return -1;

        }

        //采样位数
        switch(bit/8)
        {
        case 1:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
                break ;
        case 2:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
                break ;
        case 3:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S24_LE);
                break ;

        }

        rc=snd_pcm_hw_params_set_channels(handle, params, channels); //设置声道,1表示单声>道，2表示立体声

        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_channels:");
                return -1;
        }
        val = frequency;
        rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir); //设置>频率
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_set_rate_near:");
                return -1;
        }


        rc = snd_pcm_hw_params(handle, params);
        if(rc<0)
        {
            perror("\nsnd_pcm_hw_params: ");
            return -1;
        }

        rc=snd_pcm_hw_params_get_period_size(params, &frames, &dir); /*获取周期
长度*/
        if(rc<0)
        {
                perror("\nsnd_pcm_hw_params_get_period_size:");
                return -1;
        }

        size = frames * datablock; /*4 代表数据快长度*/
        
        buffer = (char*)malloc(size);
	int speech_len = 0;
	int nFlag = -1;

        while (1)
        {
		
                memset(buffer,0,size);
             
		if( wav_hdr->data_size - speech_len <= size )
		{	
			nFlag = 0;
                        memcpy(buffer,synth_speech+speech_len,wav_hdr->data_size - speech_len);
		}
		else
			memcpy(buffer,synth_speech+speech_len,size);

                if(nFlag == 0)
                {
                        //printf("歌曲写入结束\n");
                        break;
                }
                 else if (ret != size)
                {
                 }

           // 写音频数据到PCM设备
           while(ret = snd_pcm_writei(handle, buffer, frames)<0)
           {
                 printf("data ret = %d \n",ret);
                 usleep(2000);
                 if (ret == -EPIPE)
                 {
                      /* EPIPE means underrun */
                      fprintf(stderr, "underrun occurred\n");

                      //完成硬件参数设置，使设备准备好
                      snd_pcm_prepare(handle);
                 }
                 else if (ret < 0)
                 {
                          fprintf(stderr,
                      "error from writei: %s\n",
                      snd_strerror(ret));
                 }
           }
          speech_len = speech_len + size;
     }

        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(buffer);
        return 0;
}
/*播放器完*/

pthread_mutex_t mutex;
std::queue<char*,std::list<char*> > myqueue;
list<char*> mylist;

void* threadfun(void * arg)
{

    char buf[BUFF_LEN];  //接收缓冲区，16＊1024字节
    int server_fd, ret;
    struct sockaddr_in ser_addr; 
    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
	ret = -1;
        return &ret;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
	ret = -1;
        return &ret;
    }	


    socklen_t len;
    int count=0;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息

    char* curBuf = NULL;

    len = sizeof(clent_addr);

    //std::queue<int,list<int> >::iterator itr;
    //queue<int> q;
    while(1)
    {
	    bzero(buf, sizeof buf);

        count = recvfrom(server_fd, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        
        if(count <= 0)
        {
            printf("recieve data fail!\n");
            continue;
        }

        static unsigned int size = 0;

        size = mylist.size();

        if(size > 0){
            pthread_mutex_lock(&mutex);
            list<char*>::iterator itr = find(mylist.begin(),mylist.end(),buf);

            if( itr != mylist.end() )
            {
                continue;
            }
            pthread_mutex_unlock(&mutex);
        }else{
            curBuf = (char*)malloc(count+1);
            bzero(curBuf,count+1);
            memcpy(curBuf,buf,count);

            printf("客户端说:%s\n",buf);
            pthread_mutex_lock(&mutex);
            mylist.push_back(curBuf);
            pthread_mutex_unlock(&mutex);
       }
    }
}


int main(int argc, char* argv[])
{

	wave_pcm_hdr wav_hdr;
	char* 	     synth_speech = NULL;
	int         ret   = MSP_SUCCESS;
	const char* login_params         = "appid = 54a61231, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
	/*
	* rdn:           合成音频数字发音方式54a61231
	* volume:        合成音频的音量
	* pitch:         合成音频的音调
	* speed:         合成音频对应的语速
	* voice_name:    合成发音人
	* sample_rate:   合成音频采样率
	* text_encoding: 合成文本编码格式
	*
	*/
    system("rm ./msc/4320f99a640fa1796de3bb25c7dd8c05/* -rf");
	const char* session_begin_params = "engine_type = local,voice_name=xiaoyan, text_encoding = UTF8, tts_res_path = fo|res/tts/xiaoyan.jet;fo|res/tts/common.jet, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
   
    char* msg = "";
    pthread_mutex_init(&mutex,NULL);
	pthread_t thid; 
    if(pthread_create(&thid,NULL,threadfun,NULL)!=0) //创建线程
	{ 
		printf("thread creation failed\n"); 
		exit(1); 
    }
 	

    bool bPlayOver = true;
    unsigned int nTTSCnt = 0;
    while(1)
	{	
        if(!mylist.empty()/*myqueue.empty()*/)
        {
            pthread_mutex_lock(&mutex);
            /* 用户登录 */
            ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
            if (MSP_SUCCESS != ret)
            {
                printf("MSPLogin failed, error code: %d.\n", ret);
            } 
            ret = text_to_speech(mylist.front(),session_begin_params, &wav_hdr,&synth_speech);
            MSPLogout(); //登录失败，退出登录
            pthread_mutex_unlock(&mutex);
            free( mylist.front() );
            mylist.pop_front();
            if (MSP_SUCCESS != ret)
            {
                  printf("text_to_speech failed, error code: %d.\n", ret);
            }
            else
            {
                /*语音播放*/
                int nPlayRet = set_pcm_play(&wav_hdr,synth_speech);

                if(nPlayRet != 0)
                {
                    printf("set_pcm_play error\n");
                }
            }
        }
        else
        {
            sleep(1);
            continue;
        }
		
    }
    
    return 0;
}
