/* ekhotest.cpp */
#include <stdio.h>
//#include "config.h" // this file is generated when building Ekho
#include "ekho.h"
#include "aiui.h" 

using namespace ekho;

int local_tts_ekho(char *name, char *text)
{
	int speed,pitch,volume,rate;
	int i;
	//ekho支持粤语Cantonese、普通话Mandarin国语和韩语Korean，在这指定
	Ekho wong("Mandarin");
	wong.setVolume(100);
	if(!strcmp(name,VAKI))
	{
		wong.setPitch(20);
		wong.setRate(100);	
	}else if(!strcmp(name,DAHUANGFENG)){
		wong.setPitch(-80);
		wong.setRate(60);	
	}
	wong.blockSpeak(text);
	return 0;
}
/*
int main(int argc, char **argv)
{
	int speed,pitch,volume,rate;
	int i;
	//ekho支持粤语Cantonese、普通话Mandarin国语和韩语Korean，在这指定
	Ekho wong("Mandarin");
	wong.setVolume(100);
	wong.setPitch(pitch);
	wong.setRate(rate);
	wong.blockSpeak("欢迎光临！ 任晓亮  你好！");
	return 0;
}
*/