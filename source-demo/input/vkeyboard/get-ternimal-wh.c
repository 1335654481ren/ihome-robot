#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<termios.h>
#include<signal.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    struct winsize info;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&info);
    printf("当前终端为%d行%d列\n",info.ws_row,info.ws_col);
    return 0;
}
