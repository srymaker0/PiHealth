/*************************************************************************
	> File Name: reactor.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 12 22:23:28 2021
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H
// 客户的请求
struct logRequest {
    char name[20];
    int sex;          // 性别 1 男 0 女
};

// 服务端给客户的回应
struct logResponse {
    int flag;         // 1 正确连接，0 出错
    char msg[512];    // 回应消息
};

//写在reactor.h中  存储用户信息 由两个从反应堆维护
struct User {
    int fd;
    char name[20];
    char ip[20];
    int online;
    int epollfd;   // 用户存在的反应堆
};
#endif
