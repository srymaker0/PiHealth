/*************************************************************************
	> File Name: pihealth.h
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Oct 2021 08:45:25 PM CST
 ************************************************************************/

#ifndef _PIHEALTH_H
#define _PIHEALTH_H

#define PI_HEART 0x01
#define PI_ACK 0x02
#define PI_FIN 0x04

struct pihealth_msg_ds{
    int type;
};

struct client_ds {
    int sockfd;
    int isonline;               // >0表示在线 数字为心跳次数
    struct sockaddr_in addr;
};
void *work_on_reactor(void *arg);
void heart_beat(int signum);
void *do_login(void *arg);
#endif
