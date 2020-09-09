#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>

#include "ARS408_can_database_ch0.h"
#include "ARS408_can_database_ch1.h"
#include "ARS408_can_database_ch2.h"
#include "ARS408_can_database_ch3.h"
#include "ARS408_can_database_ch4.h"
#include "ARS408_can_database_ch5.h"
#include "ARS408_can_database_ch6.h"
#include "ARS408_can_database_ch7.h"
#include "ChangAn_CS55.h"

typedef struct CLIENT_INFO {
  int fd;
  int port;
  int state;
  pthread_t pid;
}C_INFO;

C_INFO can_port[4];
can_obj_ars408_can_database_ch0_h_t srr0;
can_obj_ars408_can_database_ch1_h_t srr1;
can_obj_ars408_can_database_ch2_h_t srr2;
can_obj_ars408_can_database_ch3_h_t srr3;
can_obj_ars408_can_database_ch4_h_t srr4;
can_obj_ars408_can_database_ch5_h_t srr5;
can_obj_ars408_can_database_ch6_h_t srr6;
can_obj_ars408_can_database_ch7_h_t srr7;
can_obj_changan_cs55_h_t cs55;

FILE *fp_log;

static inline uint64_t reverse_byte_order(uint64_t x) {
    x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
    x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
    x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
    return x;
}

void* client_fun(void *arg)
{
    int recvlen = 0;
    unsigned char recvbuf[1024] = "";
    C_INFO *client = (C_INFO *)arg;
    //printf("client:%d running ok ! \n",client->port);
    while((recvlen = recv(client->fd,recvbuf,sizeof(recvbuf),0))>0)
    {
        //printf("client:%d =  len: %d\n",client->port, recvlen);
        //for(int j= 0; j < recvlen; j++)
        //    printf("%02x ",recvbuf[j]);
        //printf("\n");
        for(int cnt = 0; cnt < recvlen; ){
            uint8_t dlc = recvbuf[cnt] & 0x0f;
            //printf("dlc = %d\n", dlc);
            const unsigned long id = recvbuf[cnt+3] << 8 | recvbuf[cnt+4];
            //printf("id = 0x%02x\n", id);
            uint64_t data = 0;
            for(int i = 0; i < 8; i++)
            {
                data = data << 8;
                data |= recvbuf[cnt+5+i];
            }
            data = reverse_byte_order(data);
            //printf("data = %016lx\n", data);
            if ( client->port == 4001 ) {
                if( ((id >> 4) & 0x0f ) == 0 ) {
                    int err = unpack_message_ch0(&srr0, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" srr0 error \n");
                    else {
                        print_message_ch0(&srr0, id, fp_log);
                        double speed1 = 0;
                        decode_can_0x300_RadarDevice_Speed(&srr0, &speed1);
                        double yawrate1 = 0;
                        decode_can_0x301_RadarDevice_YawRate(&srr0, &yawrate1);
                        uint8_t driection1 = 0;
                        decode_can_0x300_RadarDevice_SpeedDirection(&cs55, &driection1);
                        //printf("= decode ==== speed1 = %f ,yawrate1 = %f , driection1 = %d \n",speed1,yawrate1,driection1);
                    }
                }else if( ((id >> 4) & 0x0f ) == 1 ){
                    int err = unpack_message_ch1(&srr1, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" srr1 error \n");
                }
            } else if ( client->port == 4002 ) {
                if( ((id >> 4) & 0x0f ) == 2  ) {
                    int err = unpack_message_ch2(&srr2, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" srr2 error \n");
                }else if( ((id >> 4) & 0x0f ) == 3  ){
                    int err = unpack_message_ch3(&srr3, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" srr3 error \n");
                }
            } else if ( client->port == 4003 ) {
                if( ((id >> 4) & 0x0f ) == 4 ) {
                    int err = unpack_message_ch4(&srr4, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" srr4 error \n");
                }
            } else if ( client->port == 4004 ) {
                if(id == 0x278 || id == 0x218 || id == 0x208 ) {
                    int err = unpack_message(&cs55, id, data, dlc, 15363465765);
                    if( err < 0)
                        printf(" changan_cs55 error \n");
                    //else
                    //    print_message(&cs55, id, fp_log);
                    
                    double speed = 0;
                    decode_can_0x218_ESP_VehicleSpeed(&cs55, &speed);
                    double yawrate = 0;
                    decode_can_0x278_ESP_YawRate(&cs55, &yawrate);
                    uint8_t driection = 0;
                    decode_can_0x208_Wheel_Speed_FL_Direction(&cs55, &driection);
                    printf("===== speed = %f ,yawrate = %f , driection = %d \n",speed,yawrate,driection);
                    encode_can_0x300_RadarDevice_Speed(&srr0, speed);
                    encode_can_0x300_RadarDevice_SpeedDirection(&srr0, driection);
                    encode_can_0x301_RadarDevice_YawRate(&srr0, yawrate);
                    
                    uint64_t data_yawrate;
                    pack_message_ch0(&srr0, 0x301, &data_yawrate);
                    uint64_t data_speed;
                    pack_message_ch0(&srr0, 0x300, &data_speed);

                    unsigned char can_data[13] = {0x08,0x00,0x00,0x03,0x00};
                    char *ptr = (char *)&data_speed;
                    for(int i = 0; i < 8; i++)
                        can_data[i+5] = ptr[i];

                    int len = send(can_port[2].fd,can_data,sizeof(can_data),0);
                    if( len  > 0 ){
                        printf("send %d %d ok!\n", can_port[2].port,len);
                    } 
                }
            }
            cnt += 13;
        }
    }
    client->state = 0;
    printf("client: %d closed!\n",client->port);
    close(client->fd);
    return NULL;
}

int create_socket_connect(struct hostent *host, int portnumber) {
    int sockfd;
    struct sockaddr_in server_addr;
    if( (sockfd=socket(AF_INET,SOCK_STREAM,0) ) == -1 )
    {
       fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
       return -1;
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
       fprintf(stderr,"Connect error:%s\n",strerror(errno));
       return -1;
    } else {
        return sockfd;
    }
}

int main(int argc,char *argv[])
{   
    char sendbuffer[200];
    char recvbuffer[200];

    struct hostent *host;
    int port = 4001,nbytes;
    // if ( argc != 3 )
    // {
    //     fprintf(stderr,"Usage :%s hostname portnumber\a\n",argv[0]);
    //     exit(1);
    // }
    // if((host = gethostbyname(argv[1]))==NULL)
    // {
    //     herror("Get host name error\n");
    //     exit(1);
    // }
    // if((port = atoi(argv[2]))<0)
    // {
    //     fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);
    //     exit(1);
    // }
    fp_log = fopen("test.txt", "w+");

    if((host = gethostbyname("192.168.0.178"))==NULL)
    {
        herror("Get host name error\n");
        exit(1);
    }
    for( int i = 0; i < 4; i++){
        can_port[i].port = port + i;
        can_port[i].fd = create_socket_connect(host, can_port[i].port);
        if( can_port[i].fd > 0 ) {
            can_port[i].state = 1;
            if( pthread_create(&can_port[i].pid, NULL, client_fun, (void *)&can_port[i]) < 0)
            {
                printf("pthread_create Failed : %s\n",strerror(errno));
            }
            printf("Create %d %d ---Thread Success!\n", can_port[i].fd ,can_port[i].port);
        } else {
            can_port[i].state = 0;
        }
    }


    // unsigned char recvbuf[13] = {0x08,0x00,0x00,0x03,0xff,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    // int recvlen = 13;
    // int cnt = 0;
    // int dlc = recvbuf[cnt] & 0x08;
    // printf("dlc = %d\n", dlc);
    // int id = recvbuf[cnt+3] << 8 | recvbuf[cnt+4];
    // printf("id = 0x%x\n", id);
    // uint64_t x = 0;
    // for(int i = 0; i < dlc; ++i)
    // {
    //     x = x << 8;
    //     x = x | recvbuf[cnt + 5 + i];
    //     printf("data = %16lx\n", x);
    // }
    // x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
    // x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
    // x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;

    // printf("data = %16lx\n", x);
    
    // printf("\n");

    while(1)
    {
        unsigned char data[13] = {0x08,0x00,0x00,0x03,0xff,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
        for( int i = 0; i < 4; i++){
            if( can_port[i].state == 1) {
              //int len = send(can_port[i].fd,data,sizeof(data),0);
              //if( len  > 0 ){
                //printf("send %d %d ok!\n", can_port[i].port,len);
              //} 
            }
            sleep(1);
        }
    }
    exit(0);
}

