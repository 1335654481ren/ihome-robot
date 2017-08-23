#include <linux/input.h>  
#include <linux/uinput.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdint.h>  
#include <stdio.h>  
  
int reportkey(int fd, uint16_t type, uint16_t keycode, int32_t value)  
{  
    int ret;  
  
    struct input_event ev;  
  
    memset(&ev, 0, sizeof(struct input_event));  
  
    ev.type = type;  
    ev.code = keycode;  
    ev.value = value;  
  
    ret = write(fd, &ev, sizeof(struct input_event));  
    if (ret < 0) {  
        printf("report key error!\n");  
        return ret;  
    }  
  
/*  printf("key is %d %d\n", keycode, value);*/  
  
    return 0;  
}  
  
int main(void)  
{  
    struct uinput_user_dev uidev;  
  
    int fd, ret;  
  
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);  
  
    if (fd < 0) {  
        return fd;  
    }  
  
    ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);  
    ret = ioctl(fd, UI_SET_EVBIT, EV_SYN);  
  
    ret = ioctl(fd, UI_SET_KEYBIT, KEY_D);  
  
    memset(&uidev, 0, sizeof(struct uinput_user_dev));  
  
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");  
    uidev.id.bustype = BUS_USB;  
    uidev.id.vendor = 0x1234;  
    uidev.id.product = 0xfedc;  
    uidev.id.version = 1;  
  
    ret = write(fd, &uidev, sizeof(struct uinput_user_dev));  
  
    ret = ioctl(fd, UI_DEV_CREATE);  
    if (ret < 0) {  
        close(fd);  
        return ret;  
    }  
  
    while (1) {  
        reportkey(fd, EV_KEY, KEY_D, 1);  
        reportkey(fd, EV_SYN, SYN_REPORT, 0);  
        reportkey(fd, EV_KEY, KEY_D, 0);  
        reportkey(fd, EV_SYN, SYN_REPORT, 0);  
        usleep(1000);  
    }  
  
    ioctl(fd, UI_DEV_DESTROY);  
  
    close(fd);  
  
    return 0;  
}  