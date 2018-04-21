#!/bin/sh
#http://www.jianshu.com/p/ba416d2c2d52
#gcc main.c -o app -I./ -L./ -lairkiss -lairkiss_aes -lairkiss_aes_log -lairkiss_log -lpthread -Werror
gcc main.c -o app -I./ -L./ -lairkiss_log -lpthread -Werror

