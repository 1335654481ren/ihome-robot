/* ekhotest.cpp */
#include "config.h" // this file is generated when building Ekho
#include "ekho.h"

using namespace ekho;

int main(int argc, char **argv)
{

  /*ekho支持粤语Cantonese、普通话Mandarin国语和韩语Korean，在这指定*/

  Ekho wong("Mandarin");

  wong.blockSpeak("你好，欢迎！");

  return 0;

}
