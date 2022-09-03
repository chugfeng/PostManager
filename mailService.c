#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#include <windows.h>

#include "mailService.h"
#include "pojo.h"


char mailFilePath[MAXPATH];
FILE* mailFile = NULL;


//初始化接口
int MailInitApi()
{
	sprintf(mailFilePath, "%s\\data\\mails.txt", AppPath);

	mailFile = fopen(mailFilePath, "r");

	if (mailFile == NULL)
	{
		printf("系统缺失重要文件，请检查，文件路径->%s", mailFilePath);
		return 0;
	}
	fclose(mailFile);
	return 1;
}

//读取登陆者的私信，需要传入一个链表的头节点
void GetMailListApi(MailLink *mainList)
{
	int no = 0;
	MailLink *p = mainList;
	char mailBuffer[MAXFILEBUFFER];
	mailFile = fopen(mailFilePath, "r");

	//读取登录者的文件
	while (1)
	{
		mailBuffer[0] = '\0';
		if (fgets(mailBuffer, MAXFILEBUFFER, mailFile) == NULL) break;

		if (mailBuffer[0] == '\n') continue;

		int i = 0;
		//recive;mag;from;key
		//获取接收者
		char reciveId[USERIDLENTH] = { 0 };
		for (; mailBuffer[i] != '#'; i++)
			reciveId[i] = mailBuffer[i];

		//判断是否属于登录者的
		if (strcmp(reciveId, logonUser.uid))
			continue;

		//属于登录者的接下来获取消息，发送方，密钥.i++;
		i++;
		char msg[MAX_MSG_SIZE] = { 0 };
		for (int j = 0 ; mailBuffer[i] != '#'; i++, j++)
			msg[j] = mailBuffer[i];

		i++;
		char fromId[USERIDLENTH] = { 0 };
		for (int j = 0; mailBuffer[i] != '#'; i++, j++)
			fromId[j] = mailBuffer[i];

		i++;
		char fromKey[MAXMLENGTH] = { 0 };
		for (int j = 0; mailBuffer[i] != '#'; i++, j++)
			fromKey[j] = mailBuffer[i];

		MailLink* node = (MailLink*)malloc(sizeof(MailLink));
		node->next = NULL;
		node->mail.no = ++no;
		strcpy(node->mail.receiveUId, reciveId);
		strcpy(node->mail.msg, msg);
		strcpy(node->mail.fromUId, fromId);
		strcpy(node->mail.key,fromKey);

		p->next = node;
		p = p->next;
	}
	fclose(mailFile);
}

//发送私信
/**
 * 发送消息需要一个收件人，和发送的明文信息,再加一个收件人的公钥.
 * */
int SendMailApi(char reciver[], char msg[], RsaPubKey pubKey)
{

	int ques = 0;
	printf("是否确认发送？(1:发送 || 0:不发送)\n->:");

	while (1)
	{
		fflush(stdin);
		int result = scanf("%d", &ques);

		if (result == 0 || ques < 0 || ques >1)
		{
			printf("请选择1或者0\n");
			Sleep(1000);
			continue;
		}

		if (ques == 0) return -1;
		break;
	}

	int result = 0;
	//第一步-生成des密钥key
	char key[KEYLENTH] = {0};
	//char key[KEYLENTH] = "aaaaaaa";
	GetRand8Char(key);

	
	//第二部-用des将消息加密
	char keyMsg[MSGLENTH] = { 0 };
	DES_Encrypt(msg, strlen(msg), key, keyMsg);

	//第三步-用收件人的公钥通过rsa算法将key加密
	long keyDesKey[KEYLENTH] = { 0 };
	RsaEncipher(key, KEYLENTH, keyDesKey, pubKey);

	//保存ras加密后的密钥
	char keyMiBuffer[1024] = { 0 };
	char keyMiTemp[8] = { 0 };
	
	for (int i = 0; i < (sizeof(key) / sizeof(char)) /*mikey[i] != 0*/; i++)
	{
		//防止出现0
		if (keyDesKey[i] == 0) break;

		sprintf(keyMiTemp, "%d", keyDesKey[i]);

		if (i == 0)
			sprintf(keyMiBuffer, "%s", keyMiTemp);
		else
			sprintf(keyMiBuffer, "%s%s%s", keyMiBuffer, "@", keyMiTemp);
	}

	//生成一个字符数组 将收件人id 加密后的消息 加密后的key 按照 uid#key#fromid#msg保存
	char addBuffer[FILEBUFFER] = { 0 };
	//uid;msg;from;key

	sprintf(addBuffer, "%s#%s#%s#%s#\n", reciver, keyMsg, logonUser.uid, keyMiBuffer);

	//处理addBuffer 将\换成\\

	for (int i = 0; i < strlen(addBuffer); i++)
	{
		//0123456789
		if (addBuffer[i] == '\\')
		{
			for (int j = strlen(addBuffer); j > i; j--)
				addBuffer[j] = addBuffer[j - 1];
			i++;
		}
	}

	mailFile = fopen(mailFilePath, "a+");
	int add = fputs(addBuffer, mailFile);

	fclose(mailFile);

	if (add == EOF )
		return 0;
	else
		return 1;
}

//随机产生32个字符作为key
void GetRand8Char(char *a)
{
	srand((unsigned)time(NULL)); //用时间做种，每次产生随机数不一样
	int index = 0;
	while (index < KEYLENTH)
	{
		//随机产生 32位大写字符
		char c = rand() % 25 + 65;
		a[index++] = c;
	}
}
