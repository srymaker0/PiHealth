/*************************************************************************
	> File Name: pihealth.h
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Oct 2021 08:45:25 PM CST
 ************************************************************************/

#ifndef _PIHEALTH_H
#define _PIHEALTH_H
struct client_ds {
    int sockfd;
    int isonline;
    struct sockaddr_in addr;
};
void *do_login(void *arg);
#endif
