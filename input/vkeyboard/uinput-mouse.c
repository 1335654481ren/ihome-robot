#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdint.h>  
#include <stdio.h>  
  
#include <linux/input.h>  
#include <linux/uinput.h>  
/* http://www.th7.cn/system/lin/201601/149191.shtml */
void mouse_move(int fd, int dx, int dy)  
{  
    struct input_event ev;  
  
    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_ABS;  
    ev.code = REL_X;  
    ev.value = dx;  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("move error\n");  
    }  
  
    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_ABS;//EV_REL;  
    ev.code = REL_Y;  
    ev.value = dy;  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("move error\n");  
    }  
  
      
    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_SYN;  
    ev.code = SYN_REPORT;  
    ev.value = 0;  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("move error\n");  
    }  
}  
  
void mouse_report_key(int fd, uint16_t type, uint16_t keycode, int32_t value)  
{  
    struct input_event ev;  
  
    memset(&ev, 0, sizeof(struct input_event));  
  
    ev.type = type;  
    ev.code = keycode;  
    ev.value = value;  
  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("key report error\n");  
    }  
}  
  
int main(void)  
{  
  
    struct uinput_user_dev mouse;  
    int fd, ret;  
  
    int dx = 0, dy = 0;  
  
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);  
    if (fd < 0) {  
	printf("open key report error\n");
        return fd;  
    }  
  
    ioctl(fd, UI_SET_EVBIT, EV_SYN);  
  
    ioctl(fd, UI_SET_EVBIT, EV_KEY);  
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);  
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);  
    ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE); 
  
    ioctl(fd, UI_SET_KEYBIT, BTN_FORWARD);   
    ioctl(fd, UI_SET_KEYBIT, BTN_BACK);

    ioctl(fd, UI_SET_EVBIT,  EV_REL);  
    ioctl(fd, UI_SET_RELBIT, REL_X);  
    ioctl(fd, UI_SET_RELBIT, REL_Y);  
  
  
    memset(&mouse, 0, sizeof(struct uinput_user_dev));  
    snprintf(mouse.name, UINPUT_MAX_NAME_SIZE, "renx-mouse");  
    mouse.id.bustype = BUS_USB;  
    mouse.id.vendor = 0x1234;  
    mouse.id.product = 0xfedc;  
    mouse.id.version = 1;  
  
    mouse.absmin[ABS_X] = 0; 
    mouse.absmax[ABS_X] = 1920;
    mouse.absfuzz[ABS_X] = 0; 
    mouse.absflat[ABS_X] = 0; 
    mouse.absmin[ABS_Y] = 0; 
    mouse.absmax[ABS_Y] = 1080;
    mouse.absfuzz[ABS_Y] = 0; 
    mouse.absflat[ABS_Y] = 0; 
	mouse.absmin[ABS_CNT] = 0;
	mouse.absmax[ABS_CNT] = 0;
	
    ret = write(fd, &mouse, sizeof(struct uinput_user_dev));  
  
    ret = ioctl(fd, UI_DEV_CREATE);  
    if (ret < 0) { 
	printf("ioctl key report error\n"); 
        close(fd);  
        return ret;  
    }  
  
  
    sleep(5); 
    mouse_report_key(fd, EV_KEY, BTN_RIGHT, 1); 
    mouse_report_key(fd, EV_KEY, BTN_RIGHT, 0); 
    mouse_report_key(fd, EV_SYN, SYN_REPORT, 0);
  
    
    while (1) {
	dx += 10;
	dy += 10; 
	printf("dx = %d dy = %d \n",dx,dy);   
        mouse_move(fd, dx, dy);  
        sleep(1);  
        mouse_report_key(fd, EV_KEY, BTN_RIGHT, 1); 
        mouse_report_key(fd, EV_KEY, BTN_RIGHT, 0); 
        mouse_report_key(fd, EV_SYN, SYN_REPORT, 0);
    }  
  
    ioctl(fd, UI_DEV_DESTROY);  
  
    close(fd);  
  
    return 0;  
}
