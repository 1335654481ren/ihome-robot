#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdint.h>  
#include <stdio.h>  
  
#include <linux/input.h>  
#include <linux/uinput.h> 

#define ALOGE(fmt, args...) printf(fmt, ##args)  
  
#define BUTTON_DOWN 1  
#define BUTTON_UP   0  

#define DIS_PIXEL 100  
#define DUR_TIME  5
/* http://www.th7.cn/system/lin/201601/149191.shtml */
int vtouch_fd;
int keyboard_fd;
struct uinput_user_dev uinput; // uInput device structure   
/* Setup the uinput device */   
int touch_uinput_setup_device()   
{   
   //int i=0;   
   int ret;  
     
   // Open the input device   
   vtouch_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);   
   if (vtouch_fd < 0){   
      ALOGE("Unable to open /dev/uinput/n");   
      return -1;   
   }   
     
   memset(&uinput,0,sizeof(uinput)); // Intialize the uInput device to NULL   
   strncpy(uinput.name, "vtouch", UINPUT_MAX_NAME_SIZE);   
   uinput.id.version = 4;   
   uinput.id.bustype = BUS_USB;   
  
  
   uinput.absmin[ABS_X] = 0;  
   uinput.absmax[ABS_X] = 1920;   
   uinput.absmin[ABS_Y] = 0;  
   uinput.absmax[ABS_Y] = 1080;   
     
   // Setup the uinput device   
   ret = ioctl(vtouch_fd, UI_SET_EVBIT, EV_ABS);   
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_EVBIT, EV_KEY);   
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_ABSBIT, ABS_X);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
     
   ret = ioctl(vtouch_fd, UI_SET_ABSBIT, ABS_Y);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_TOUCH);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_TOOL_MOUSE);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_TOOL_PEN);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   ret = ioctl(vtouch_fd, UI_SET_KEYBIT, KEY_BACK);  
   if(ret){  
      ALOGE("ioctl fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   /*     
    ioctl(vtouch_fd, UI_SET_EVBIT, EV_KEY);   
    ioctl(vtouch_fd, UI_SET_EVBIT, EV_REL);   
      
    ioctl(vtouch_fd, UI_SET_RELBIT, REL_X);   
    ioctl(vtouch_fd, UI_SET_RELBIT, REL_Y);   
    
    for (i=0; i < 256; i++) {   
       ioctl(vtouch_fd, UI_SET_KEYBIT, i);   
    }   
      
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_MOUSE);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_TOUCH);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_MOUSE);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_LEFT);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_MIDDLE);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_RIGHT);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_FORWARD);   
    ioctl(vtouch_fd, UI_SET_KEYBIT, BTN_BACK);   
    */    
  
     
   // Create input device into input sub-system   
   if(write(vtouch_fd, &uinput, sizeof(uinput))== -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
        
   if(ioctl(vtouch_fd, UI_DEV_CREATE))   
   {   
      ALOGE("Unable to create UINPUT device.");   
      return -1;   
   }   
     
   return 0;   
}

/* Destroy the uinput device */   
int touch_destroy_uinput_device()   
{  
   // Destroy the uinput device  
   ioctl(vtouch_fd, UI_DEV_DESTROY);   
     
   // Close the uinput device   
   close(vtouch_fd);  
}  
  
int touch_move_mouse(int x, int y)   
{   
   struct input_event event;   
   int ret;  
     
   // Move pointer to (0,0) location   
   memset(&event, 0, sizeof(event));   
  
   gettimeofday(&event.time, NULL);   
  
   event.type = EV_ABS;  
   event.code = ABS_X;  
   event.value = x;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
        
   event.type = EV_ABS;  
   event.code = ABS_Y;  
   event.value = y;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   event.type = EV_KEY;  
   event.code = BTN_TOOL_PEN;   
   event.value = 1;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
     
  
   event.type = EV_SYN;   
   event.code = SYN_REPORT;   
   event.value = 0;   
   ret = write(vtouch_fd, &event, sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
   return 0;  
}   
  
  
int write_touch_hide(void)   
{   
   struct input_event event;   
   int ret;  
     
   // Move pointer to (0,0) location   
   memset(&event, 0, sizeof(event));   
  
   gettimeofday(&event.time, NULL);   
  
   event.type = EV_ABS;  
   event.code = ABS_X;  
   event.value = 0;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
        
   event.type = EV_ABS;  
   event.code = ABS_Y;  
   event.value = 0;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   event.type = EV_KEY;  
   event.code = BTN_TOOL_PEN;   
   event.value = 0;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
     
  
   event.type = EV_SYN;   
   event.code = SYN_REPORT;   
   event.value = 0;   
   ret = write(vtouch_fd, &event, sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
   return 0;  
}  
  
  
  
int touch_write_key(int key, int down)   
{   
   struct input_event event;   
   int ret;  
     
   // Move pointer to (0,0) location   
   memset(&event, 0, sizeof(event));   
  
   gettimeofday(&event.time, NULL);   
  
   event.type = EV_KEY;  
   event.code = key;   
   event.value = down;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
     
  
   event.type = EV_SYN;   
   event.code = SYN_REPORT;   
   event.value = 0;   
   ret = write(vtouch_fd, &event, sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
   return 0;  
}  
  
int touch_write_button(int x, int y, int down)    
{   
   struct input_event event;   
   int ret;  
     
   // Move pointer to (0,0) location   
   memset(&event, 0, sizeof(event));   
  
   gettimeofday(&event.time, NULL);   
  
   event.type = EV_ABS;  
   event.code = ABS_X;  
   event.value = x;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
        
   event.type = EV_ABS;  
   event.code = ABS_Y;  
   event.value = y;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
     
   event.type = EV_KEY;  
   event.code = BTN_TOUCH;   
   event.value = down;  
   ret = write(vtouch_fd,&event,sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
  
   event.type = EV_SYN;   
   event.code = SYN_REPORT;   
   event.value = 0;   
   ret = write(vtouch_fd, &event, sizeof(event));  
   if(ret == -1){  
      ALOGE("write fail:%s(%d)",__FUNCTION__,__LINE__);  
      return -1;     
   }  
   return 0;  
}    
//////////////////////////////////////////////////////////////////////////////   
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

void mouse_move(int fd, int dx, int dy)  
{  
    struct input_event ev;  
  
    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_REL;  
    ev.code = REL_X;  
    ev.value = dx;  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("move error\n");  
    }  
  
    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_REL;  
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

    memset(&ev, 0, sizeof(struct input_event));  
    ev.type = EV_SYN;  
    ev.code = SYN_REPORT;  
    ev.value = 0;  
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {  
        printf("move error\n");  
    }  
}  

void setup_uinput_mouse()
{
    struct uinput_user_dev mouse;  
    int ret;  
  
    keyboard_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);  
    if (keyboard_fd < 0) {  
        printf("open key report error\n");
        return keyboard_fd;  
    }  
  
    ioctl(keyboard_fd, UI_SET_EVBIT, EV_SYN);  
  
    ioctl(keyboard_fd, UI_SET_EVBIT, EV_KEY);  
    ioctl(keyboard_fd, UI_SET_KEYBIT, BTN_LEFT);  
    ioctl(keyboard_fd, UI_SET_KEYBIT, BTN_RIGHT);  
    ioctl(keyboard_fd, UI_SET_KEYBIT, BTN_MIDDLE); 
  
    ioctl(keyboard_fd, UI_SET_KEYBIT, BTN_FORWARD);   
    ioctl(keyboard_fd, UI_SET_KEYBIT, BTN_BACK);

    ioctl(keyboard_fd, UI_SET_EVBIT,  EV_REL);  
    ioctl(keyboard_fd, UI_SET_RELBIT, REL_X);  
    ioctl(keyboard_fd, UI_SET_RELBIT, REL_Y);  
   
  
    memset(&mouse, 0, sizeof(struct uinput_user_dev));  
    snprintf(mouse.name, UINPUT_MAX_NAME_SIZE, "mouse");  
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
  
    ret = write(keyboard_fd, &mouse, sizeof(struct uinput_user_dev));  
  
    ret = ioctl(keyboard_fd, UI_DEV_CREATE);  
    if (ret < 0) { 
        printf("ioctl key report error\n"); 
        close(keyboard_fd);  
        return ret;  
    } 
} 

int main(void)  
{  
  int dx = 0, dy = 0;
  setup_uinput_mouse(); 
  touch_uinput_setup_device();
  while (1) {
    //dx += 10;
    //dy += 10; 
    printf("dx = %d dy = %d \n",dx,dy);   
    //mouse_move(keyboard_fd, dx, dy);
    touch_move_mouse(0, 0);
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 1); 
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 0); 
    sleep(1);
    printf("dx = %d dy = %d \n",dx,dy);   
    //mouse_move(keyboard_fd, dx, dy);
    touch_move_mouse(1920, 0);
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 1); 
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 0); 
    sleep(1);
    printf("dx = %d dy = %d \n",dx,dy);   
    //mouse_move(keyboard_fd, dx, dy);
    touch_move_mouse(1920, 1080);
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 1); 
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 0); 
    sleep(1);
    printf("dx = %d dy = %d \n",dx,dy);   
    //mouse_move(keyboard_fd, dx, dy);
    touch_move_mouse(1080, 1920);
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 1); 
    mouse_report_key(keyboard_fd, EV_KEY, BTN_RIGHT, 0); 
    sleep(1);

    //mouse_report_key(keyboard_fd, EV_KEY, KEY_ENTER, 1);
  }  

  ioctl(keyboard_fd, UI_DEV_DESTROY);  

  close(keyboard_fd);  

  return 0;  
}
