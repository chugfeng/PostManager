#pragma once

//����д�ӽ���,���¶����

#define KEYLENTH 32
#define MSGLENTH 32
#define FILEBUFFER 1024
#define FILEPATH 128

#define NOTICELENTH 256
#define USERIDLENTH 9
#define TIMELENTH 24

//����Ļᱻ�𲽴���

#define MAX_MSG_SIZE 10240
#define MAXMLENGTH 1024

char AppPath[FILEPATH];

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
	char key[256];
}Mail;

//��������˽������
typedef struct MailNode
{
	Mail mail;
	struct MailNode *next;
}MailLink;

//��ϵ�˽ڵ�
typedef struct MailUserNode
{
	int no;
	char uid[8];
	//��ϵ�˵Ĺ�Կ
	int e;
	int n;
}MailUserNode;

//��ϵ������
typedef struct MailUserLink
{
	MailUserNode user;
	struct MailUserLink* next;
}MailUserLink;

//����Ľڵ�
typedef struct NoticeNode
{
	int no;
	char uid[USERIDLENTH];
	char notice[NOTICELENTH];
	char publishTiem[TIMELENTH];
	char keyMd5[33];
}NoticeNode;

//���������
typedef struct NoticeLink
{
	NoticeNode notice;
	struct NoticeLink* next;
}NoticeLink;

