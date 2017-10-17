/***************************************************************
文件命名： brain.c
函数功能：向图tuling发送请求，并接受返回的结果
编程人员：renxiaoliang
编程时间：2017-06-05 14:54:42
修改时间：
版本编号：v1.0.0
技术支持：liangxiao.ren@qq.com
测试方法：./tuling_test ,然后输入要测试的命令(例如:讲一段相声)
****************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "cJSON.h"

#include <math.h>
#include "ros/ros.h"
#include "robot_msgs/Ear.h"
#include <robot_msgs/Speek.h> 
#include <cstdlib>  
#include "aiui.h"


#define PORT 80
#define SIZE 1024*16

//key   : http://www.tuling123.com/ 申请到的APIkey,共32位
//userid:在官网未找到，这里是随便写的。
char request_data1[100] = "{\"key\":\"23771317366d42ca8cbbc978a6a87364\",\"userid\":\"123456\",\"info\":\"";
char request[SIZE] = {0};
ros::ServiceClient client;
ros::Subscriber listence;

void listenCallback(const robot_msgs::Ear::ConstPtr& msg)   //callback函数，收到msg时调用
{
  printf("get data :%s\n",msg->data.c_str());
//  ROS_INFO("I heard: [%s]", msg->data.c_str());
}
 
int json_pasre(char * jsonStr)
{
    cJSON * pRoot = NULL;
    cJSON * item = NULL;//cjson对象
    cJSON *pContent = NULL;
    cJSON *pType = NULL;//cjson对象
	int eventType = 0;
	pRoot = cJSON_Parse(jsonStr);
	printf("data to json-------------\n");
    if (!pRoot) 
    {
        printf("Error before: error[%s]\n",cJSON_GetErrorPtr());
		return -1;
    }
    else
    {
        //printf("%s\n", "有格式的方式打印Json:");           
        printf("%s\n\n", cJSON_Print(pRoot));
 		pContent = cJSON_GetObjectItem(pRoot, "code");//
 		pType = cJSON_GetObjectItem(pRoot, "text");//
		if (NULL != pType)
		{
			eventType = atoi(cJSON_Print(pContent));
			//printf("----------------%d\n",eventType);
			if(eventType == 100000)
			{
				printf("recognize words:%s\n",cJSON_Print(pType));
				std::stringstream arg1;
				std::stringstream arg2;
				robot_msgs::Speek srv;	
				srv.request.id = ACTION;  
				srv.request.type = TODO_LOCAL_TTS;	
				arg1 << XIAOFENG;
				arg2 << cJSON_Print(pType);
				srv.request.arg1 = arg1.str();
				srv.request.arg2 = arg2.str();
				if (client.call(srv))  
				{  
					ROS_INFO("code: %d", srv.response.code);
				}  
				else  
				{  
					ROS_ERROR("Failed to call service add_two_ints");  
				}
			}
		}
    }
	cJSON_Delete(pRoot);
    return 0;    
}

int get_json(char *str_data,int length)
{
	char buff[1024*1024];
	int i = 0,j = 0;
	int flag = 0;
	for(i = 0; i <length; i ++ )
	{
		if(str_data[i]== '{')
			flag = 1;
		if(flag == 1)
			buff[j++] = str_data[i];
		if(str_data[i] == '}')
			buff[j] = '\0';
	}
	//printf("get json = %s\n-----n",buff);
	json_pasre(buff);
}

int main(int argc, char **argv)
{
    int sockfd, ret, fd,len;
    struct sockaddr_in addr;
    struct hostent *pURL = NULL;
    char send_contents[SIZE];
    char input_buf[128], recv_buf[SIZE];
    char str_len[8];
    fd_set   t_set;
    struct timeval  tv;
	ros::init(argc, argv, "brain");
	ros::NodeHandle n;	
	client = n.serviceClient<robot_msgs::Speek>("/speek");  
	listence = n.subscribe("/aiui_ear", 10, listenCallback);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
    {
        printf("error:%s\n",strerror(errno));
        return -1;
    }

    pURL = gethostbyname("www.tuling123.com");  //获取IP地址等信息

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr_list[0]);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("fail:%s\n",strerror(errno));
        goto END;
    }
	
    printf("connected sucuessfull\n");
    
    FD_ZERO(&t_set);
    FD_SET(sockfd, &t_set);
    tv.tv_sec= 5;
    tv.tv_usec= 0;

    while(ros::ok())
    {
        memset(input_buf, 0, 128);
        printf("input you words：");
        scanf("%s",input_buf);
        if(!strcmp(input_buf,"quit") || !strcmp(input_buf,"q"))
        {
            goto END;
        }
        else if(!strcmp(input_buf,"test"))
        {
            strcpy(input_buf,"讲个笑话");   //终端输入的编码可能有问题，此处默认保留一份正确的UTF-8编码示例
        }
        sprintf(request,"%s%s%s",request_data1,input_buf,"\"}");
        
        memset(str_len, 0, 8);
        len = strlen(request);
        sprintf(str_len, "%d", len);

        memset(send_contents, 0, SIZE);
        strcat(send_contents, "POST /openapi/api HTTP/1.1\n");
        strcat(send_contents, "Host: www.tuling123.com\n");
        strcat(send_contents, "Content-Type: application/json; charset=UTF-8\n");
        strcat(send_contents, "Content-Length: ");
        strcat(send_contents, str_len);
        strcat(send_contents, "\n\n");
        strcat(send_contents, request); //要发送给服务器的数据部分
        strcat(send_contents, "\n\n");

        ret = send(sockfd,send_contents,strlen(send_contents),0);
        if (ret < 0) 
        {
            printf("send data fail:%s\n",strerror(errno));
            goto END;
        }
        else
        {
            //printf("*************************************\n");
            //printf("send msgs ok，messsge in total %d bytes！\n", ret);
        }
        
        memset(recv_buf, 0, SIZE);
        fd = select(sockfd+1, &t_set, NULL, NULL, &tv);
        if(fd < 0)
        {
            printf("fail:%s\n",strerror(errno));
            goto END;
        }
        else if(fd == 0)
        {
            printf("recv data timeout!!");
            continue;
        }
        
        ret = recv(sockfd, recv_buf, SIZE,0);
        if(ret < 0)
        {
            printf("recv error:%s\n",strerror(errno));
            goto END;
        }
        else
        {
        //    printf("recve message ok，get %d bytes----\n", ret);
        //    printf("----%s\n*************************************\n", recv_buf);
			get_json(recv_buf,ret);
        }
    }
END:    
    close(sockfd);
    
    return 0;
}
