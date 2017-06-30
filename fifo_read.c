#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>  
#include <assert.h>

int main()
{
    const char *fifo_name = "/tmp/my_fifo";
    fd_set fds;
    int pipe_fd = -1;
    int data_fd = -1;
    int maxfdp = -1;
    int res = 0;
    int open_mode = O_RDONLY;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;
    int bytes_write = 0;
	struct timeval timeout={1,0}; //select等待3秒，3秒轮询，要非阻塞就置0
    memset(buffer, '\0', sizeof(buffer));
    
    if(access(fifo_name, F_OK) == -1)
    {
        printf ("Create the fifo pipe.\n");
        res = mkfifo(fifo_name, 0777);

        if(res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Process %d opening FIFO O_RDONLY\n", getpid());
    pipe_fd = open(fifo_name, open_mode);
    data_fd = open("DataFormFIFO.txt", O_WRONLY|O_CREAT, 0644);
    

    if (data_fd == -1)
    {
        fprintf(stderr, "Open file[DataFormFIFO.txt] failed\n");
        close(pipe_fd);
        return -1;
    }

    printf("Process %d result %d\n",getpid(), pipe_fd);
    if(pipe_fd != -1)
    {
		while(1)  
    	{  
        	FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化  
        	FD_SET(pipe_fd,&fds); //添加描述符   
       	 	maxfdp=pipe_fd>data_fd?pipe_fd+1:data_fd+1; //描述符最大值加1  
        	if(select(maxfdp,&fds,NULL,NULL,&timeout) > 0) //select使用  
        	{   
            		if(FD_ISSET(pipe_fd,&fds)) //测试sock是否可读，即是否网络上有数据  
            		{  
                		res = read(pipe_fd, buffer, PIPE_BUF);
                		//if(res > 0){
                 			//if(FD_ISSET(data_fd,&fds)) //测试文件是否可写                			
             					bytes_write = write(data_fd, buffer, res);	                 			
                		//}
                		printf("recv:%s\n",buffer); 
            		}// end if break;  
        	}// end switch  
    	}//end while  

        close(pipe_fd);
        close(data_fd);
    }
    else
        exit(EXIT_FAILURE);

     printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
     exit(EXIT_SUCCESS);
}
