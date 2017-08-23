/* ekhotest.cpp */
#include <stdio.h>
//#include "config.h" // this file is generated when building Ekho
#include "ekho.h"
 
using namespace ekho;

int main(int argc, char **argv)
{
   int speed,pitch,volume,rate;
   int i;
  /*ekho支持粤语Cantonese、普通话Mandarin国语和韩语Korean，在这指定*/
  Ekho wong("Mandarin");
  wong.setVolume(100);
/*  for( pitch = -10 ; pitch < 100; pitch++ )
  {
	for( rate = -20 ; rate < 50 ; rate ++)
	{
		wong.setPitch(pitch);
		wong.setRate(rate);
		printf("pitch = %d rate = %d\n",pitch,rate);
		//for( i = 0 ; i < 2; i ++)
		{
			wong.blockSpeak("欢迎光临！ 任晓亮  你好！");	
		}	
	}
  } 
*/
  
	wong.setPitch(-80);
	wong.setRate(60);
	wong.blockSpeak("欢迎光临！任晓亮  你好");	
	
	return 0;
}
