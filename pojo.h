#pragma once

#include <stdio.h>

//����д�ӽ���,���¶����

#define KEYLENTH 32
#define MSGLENTH 32
#define FILEBUFFER 1024

//����һ�����ӷ�
#define symbol #

//����Ļᱻ�𲽴���

#define MAX_MSG_SIZE 10240
#define MAXPATH 1024
#define MAXFILEBUFFER 256
#define MAXUSERID 128
#define MAXPASS 128
#define MAXMLENGTH 1024

#define USER_ID_LENTH 16
#define KEY_LENTH 8

char AppPath[MAXPATH];

//��¼�ߵ���Ϣ
struct logon
{
    char uid[8];
}logonUser;

//��������˽�ŵĽṹ��
typedef struct Mail
{
	int no;
	char fromUId[8];
	char receiveUId[8];
	char msg[MAX_MSG_SIZE];
	char key[64];
}Mail;

//��������˽������Ľṹ��
typedef struct MailNode
{
	Mail mail;
	struct MailNode *next;
}MailLink;

//�����ʼ�ʱչʾ��ϵ���õĽڵ�
typedef struct MailUserNode
{
	int no;
	char uid[8];

	//��ϵ�˵Ĺ�Կ
	int e;
	int n;
}MailUserNode;

//��ϵ�˽ڵ�
typedef struct MailUserLink
{
	MailUserNode user;
	struct MailUserLink* next;
}MailUserLink;

