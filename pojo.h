#pragma once

#include <stdio.h>

//从新写加解密,从新定义宏

#define KEYLENTH 32
#define MSGLENTH 32
#define FILEBUFFER 1024

//定义一个链接符
#define symbol #

//下面的会被逐步代替

#define MAX_MSG_SIZE 10240
#define MAXPATH 1024
#define MAXFILEBUFFER 256
#define MAXUSERID 128
#define MAXPASS 128
#define MAXMLENGTH 1024

#define USER_ID_LENTH 16
#define KEY_LENTH 8

char AppPath[MAXPATH];

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
	char key[64];
}Mail;

//用来保存私信链表的结构体
typedef struct MailNode
{
	Mail mail;
	struct MailNode *next;
}MailLink;

//发送邮件时展示联系人用的节点
typedef struct MailUserNode
{
	int no;
	char uid[8];

	//联系人的公钥
	int e;
	int n;
}MailUserNode;

//联系人节点
typedef struct MailUserLink
{
	MailUserNode user;
	struct MailUserLink* next;
}MailUserLink;

