#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * author:ningci dev
 * date: 2017-05-04 15:58
 */
#define MOUSE_DEV "/dev/input/mouse0"

//定义步进
#define MOVE_STEP 10
static int postion_x;
static int postion_y;
static int mouse_fd;

int main(int argc, char **argv)
{
    //限定最大 X  Y 
    int max_x = 1920;
    int max_y = 1080;
    
    mouse_fd = open(MOUSE_DEV, O_RDONLY);
    if(-1 == mouse_fd)
    {
        printf("mouse cat't open %s \n", MOUSE_DEV);
        return -1;
    }
    while(1)
    {
        unsigned char buf[3];
        if(read(mouse_fd, buf, sizeof(buf)))
        {
            /**
             * 原理 当不为0 时说明鼠标在移动，经测试发现，值为 12 或 255 254 所以这里取比10小就是减少
             */
            //X 向右移动时变大 没问题
            if(0 < buf[1])
            {
                postion_x += (10 > buf[1]) ? MOVE_STEP : (0 - MOVE_STEP);
            }
            //Y 向下移动时变小 需要反转
            if(0 < buf[2])
            {
                postion_y += (10 > buf[2]) ? (0 - MOVE_STEP) : MOVE_STEP;
            }
            postion_x = (1 > postion_x) ? 0 : postion_x;
            postion_y = (1 > postion_y) ? 0 : postion_y;
            postion_x = (max_x < postion_x) ? max_x : postion_x;
            postion_y = (max_y < postion_y) ? max_y : postion_y;
            
            printf("x:%d y:%d \n", postion_x,  postion_y);
        }
    }
    return 0;
}
