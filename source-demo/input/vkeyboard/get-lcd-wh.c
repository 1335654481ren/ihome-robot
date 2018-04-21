// #include<stdio.h>
// #include<sys/types.h>
// #include<sys/stat.h>
// #include<sys/mman.h>
// #include<sys/ioctl.h>
// #include<unistd.h>
// #include<fcntl.h>
// #include<linux/fb.h>

// int main(int argc,char *argv[]){
//     int fd;
//     struct fb_var_screeninfo screen_info;
//     fd = open("/dev/fb0",O_RDWR);
//     ioctl(fd,FBIOGET_VSCREENINFO,&screen_info);
//     printf("%d*%d\n",screen_info.xres,screen_info.yres);
//     close(fd);
//     return 0;
// }
#include <stdio.h>
    #include <xcb/xcb.h>
 
    int 
    main ()
    {
        /* Open the connection to the X server. Use the DISPLAY environment variable */
 
        int i, screenNum;
        xcb_connection_t *connection = xcb_connect (NULL, &screenNum);
 
 
        /* Get the screen whose number is screenNum */
 
        const xcb_setup_t *setup = xcb_get_setup (connection);
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator (setup);  
 
        // we want the screen at index screenNum of the iterator
        for (i = 0; i < screenNum; ++i) {
            xcb_screen_next (&iter);
        }
 
        xcb_screen_t *screen = iter.data;
 
 
        /* report */
 
        printf ("\n");
        printf ("Informations of screen %ld:\n", screen->root);
        printf ("  width.........: %d\n", screen->width_in_pixels);
        printf ("  height........: %d\n", screen->height_in_pixels);
        printf ("  white pixel...: %ld\n", screen->white_pixel);
        printf ("  black pixel...: %ld\n", screen->black_pixel);
        printf ("\n");
 
        return 0;
    }