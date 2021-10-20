/*************************************************************************
	> File Name: common.h
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Tue 23 Mar 2021 09:04:32 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)   
#endif
int make_nonblock(int fd);
int make_block(int fd);
int socket_create(int port);
int socket_connect(const char *ip, int port);
char *get_conf_value(const char *filename, const char *key);
#endif
