#include <espeak/speak_lib.h>  // 包括espeak的头文件
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char word[] = "吃葡萄不吐葡萄皮";
    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);    // 初始化
    espeak_SetVoiceByName("zh+f2");    // 设置音源为中文女声
    espeak_Synth(word, strlen(word) + 1, 0, POS_CHARACTER, 0, espeakCHARS_UTF8, NULL, NULL);// 发音
    sleep(3);     // 等一段时间，否则程序会立即退出，听不到发音
    espeak_Terminate();  // 回收资源
}
