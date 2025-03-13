#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <pthread.h>
#include <time.h>

#include <json-c/json.h>

#include <sys/ioctl.h>
#include <linux/videodev2.h> //摄像头
#include <sys/mman.h>

#define MAX_SIZE 1024


enum action{REG,LOGIN,WARN,FORGOT,QT,ST,CHANGEPWD,CHANGENAME,DATA,RECV,NAME,ID,SURE,SMOKE,HUMIDITY,TEMPERATURE,LIGHT,MONITOR,THRESHOLD,BEEP};  //所实现的功能
enum result{SUCCESS = 1, FAILURE, PWERR, NOUSR, EMPTY, NOTLINE, ONLINE,SINGAL,ALL};

// #pragma pack(push, 1) // 按1字节对齐
typedef struct message
{
    int flag;                           //标志位，判断登录方式，0：用户名登录，1：ID登录
    int action;
    int num;                            //验证码
    char n_id[MAX_SIZE];                  //用户ID
    unsigned long id;            
    char n_action[MAX_SIZE];
    char name[MAX_SIZE];                //用户名
    char password[MAX_SIZE];            //用户密码
    char msg_humidity[MAX_SIZE];        //湿度
    char msg_temperature[MAX_SIZE];     //温度
    char msg_smoke[MAX_SIZE];           //烟雾浓度
    char msg_light[MAX_SIZE];           //光照强度
    char toname[MAX_SIZE];
}Msg;
// #pragma pack(pop) // 恢复默认对齐
extern Msg *msg;
//tcp服务器初始化函数
extern int tcp_server_init(const char *ip, int port, int backlog);
//服务器等待客户端连接函数
extern int tcp_server_wait(int fd); 
//给每一个连接的客户端开辟一个线程
extern void *pthread_read(void *arg);
//注册
extern int reg(int cfd, Msg *msg);
//登陆
extern int login(int cfd, Msg *msg);
//修改用户名
extern int changename(int cfd, Msg *msg);
//修改密码
extern int changepwd(int cfd, Msg *msg);
//发送环境数据给客户端
extern int send_all(int cfd, Msg *msg);
//接收环境数据
extern int recv_data(int cfd, Msg *msg);
//忘记密码
extern int forget(int cfd, Msg *msg);
//摄像头数据传输
extern int camera(int cfd, Msg *msg);
//设置阈值
extern int threshold(int cfd, Msg *msg);
//发送烟雾数据
extern int send_smoke(int cfd, Msg *msg);
//发送光照数据
extern int send_light(int cfd, Msg *msg);
//发送湿度数据
extern int send_humidity(int cfd, Msg *msg);
//发送温度数据
extern int send_temperature(int cfd, Msg *msg);
//蜂鸣器报警
extern int send_beep(int cfd, Msg *msg);

extern void generate_ipd_message(char *buffer, const char *data);

#endif
