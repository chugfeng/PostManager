#pragma once

//从新写加解密,从新定义宏

#define KEYLENTH 32
#define MSGLENTH 32
#define FILEBUFFER 1024
#define FILEPATH 128

#define NOTICELENTH 256
#define USERIDLENTH 9
#define TIMELENTH 24

//下面的会被逐步代替

#define MAX_MSG_SIZE 10240
#define MAXMLENGTH 1024

char AppPath[FILEPATH];

//登录者的信息
struct logon
{
    char uid[8];
}logonUser;

//用来保存私信的结构体
typedef struct Mail
{
	int no;
	char fromUId[8];
	char receiveUId[8];
	char msg[MAX_MSG_SIZE];
	char key[256];
}Mail;

//用来保存私信链表
typedef struct MailNode
{
	Mail mail;
	struct MailNode *next;
}MailLink;

//联系人节点
typedef struct MailUserNode
{
	int no;
	char uid[8];
	//联系人的公钥
	int e;
	int n;
}MailUserNode;

//联系人链表
typedef struct MailUserLink
{
	MailUserNode user;
	struct MailUserLink* next;
}MailUserLink;

//公告的节点
typedef struct NoticeNode
{
	int no;
	char uid[USERIDLENTH];
	char notice[NOTICELENTH];
	char publishTiem[TIMELENTH];
	char keyMd5[33];
}NoticeNode;

//公告的链表
typedef struct NoticeLink
{
	NoticeNode notice;
	struct NoticeLink* next;
}NoticeLink;

