#include <stdio.h>
#include <strings.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <cstddef>
#include <vector>
//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  str+=pattern;//扩展字符串以方便操作
  int size=str.size();
 
  for(int i=0; i<size; i++)
  {
    pos=str.find(pattern,i);
    if(pos<size)
    {
      std::string s=str.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
  return result;
}

int main(int argc, char * argv[])
{

	//1 创建udp通信socket
	int udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_socket_fd == -1)
	{
		perror("socket failed!\n");
		return -1;
	}
	//设置目的IP地址
    struct sockaddr_in dest_addr = {0};
    dest_addr.sin_family = AF_INET;//使用IPv4协议
    dest_addr.sin_port = htons(8989);//设置接收方端口号
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //设置接收方IP 
	
	
    char buf[1024] = {0};

    std::string cmd;
    //cmd = "(standard input):2025:08-19 15:28:55.073 I/MICO.speech( 1010): [SpeechEventHandler] [SpeechManager]:MSG_ASR_FINAL_RESULT:停车场";
	std::cin >> cmd;
	std::vector<std::string> result = split(cmd,":");
	std::cout << cmd << std::endl;
	std::cout<<"The result:"<<std::endl;
	for(int i=0; i<result.size(); i++)
	{
		std::cout <<i << ":" << result[i] << std::endl;
	}
	sendto(udp_socket_fd, result[7].c_str(), result[7].size(), 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
	//2 循环发送数据
  //   while (1)
  //   {
		// printf("Please input msg:");
		// scanf("%s",buf);//从键盘输入获取要发送的消息
  //       sendto(udp_socket_fd, buf, strlen(buf), 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr)); 
  //       if(strcmp(buf, "exit") == 0)
		// {
		// 	break;//退出循环
		// }
		// memset(buf,0,sizeof(buf));//清空存留消息
  //   }
	
	//3 关闭通信socket
    //close(udp_socket_fd);
    return 0;
}