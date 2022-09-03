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


//��ʼ���ӿ�
int MailInitApi()
{
	sprintf(mailFilePath, "%s\\data\\mails.txt", AppPath);

	mailFile = fopen(mailFilePath, "r");

	if (mailFile == NULL)
	{
		printf("ϵͳȱʧ��Ҫ�ļ������飬�ļ�·��->%s", mailFilePath);
		return 0;
	}
	fclose(mailFile);
	return 1;
}

//��ȡ��½�ߵ�˽�ţ���Ҫ����һ�������ͷ�ڵ�
void GetMailListApi(MailLink *mainList)
{
	int no = 0;
	MailLink *p = mainList;
	char mailBuffer[MAXFILEBUFFER];
	mailFile = fopen(mailFilePath, "r");

	//��ȡ��¼�ߵ��ļ�
	while (1)
	{
		mailBuffer[0] = '\0';
		if (fgets(mailBuffer, MAXFILEBUFFER, mailFile) == NULL) break;

		if (mailBuffer[0] == '\n') continue;

		int i = 0;
		//recive;mag;from;key
		//��ȡ������
		char reciveId[USERIDLENTH] = { 0 };
		for (; mailBuffer[i] != '#'; i++)
			reciveId[i] = mailBuffer[i];

		//�ж��Ƿ����ڵ�¼�ߵ�
		if (strcmp(reciveId, logonUser.uid))
			continue;

		//���ڵ�¼�ߵĽ�������ȡ��Ϣ�����ͷ�����Կ.i++;
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

//����˽��
/**
 * ������Ϣ��Ҫһ���ռ��ˣ��ͷ��͵�������Ϣ,�ټ�һ���ռ��˵Ĺ�Կ.
 * */
int SendMailApi(char reciver[], char msg[], RsaPubKey pubKey)
{

	int ques = 0;
	printf("�Ƿ�ȷ�Ϸ��ͣ�(1:���� || 0:������)\n->:");

	while (1)
	{
		fflush(stdin);
		int result = scanf("%d", &ques);

		if (result == 0 || ques < 0 || ques >1)
		{
			printf("��ѡ��1����0\n");
			Sleep(1000);
			continue;
		}

		if (ques == 0) return -1;
		break;
	}

	int result = 0;
	//��һ��-����des��Կkey
	char key[KEYLENTH] = {0};
	//char key[KEYLENTH] = "aaaaaaa";
	GetRand8Char(key);

	
	//�ڶ���-��des����Ϣ����
	char keyMsg[MSGLENTH] = { 0 };
	DES_Encrypt(msg, strlen(msg), key, keyMsg);

	//������-���ռ��˵Ĺ�Կͨ��rsa�㷨��key����
	long keyDesKey[KEYLENTH] = { 0 };
	RsaEncipher(key, KEYLENTH, keyDesKey, pubKey);

	//����ras���ܺ����Կ
	char keyMiBuffer[1024] = { 0 };
	char keyMiTemp[8] = { 0 };
	
	for (int i = 0; i < (sizeof(key) / sizeof(char)) /*mikey[i] != 0*/; i++)
	{
		//��ֹ����0
		if (keyDesKey[i] == 0) break;

		sprintf(keyMiTemp, "%d", keyDesKey[i]);

		if (i == 0)
			sprintf(keyMiBuffer, "%s", keyMiTemp);
		else
			sprintf(keyMiBuffer, "%s%s%s", keyMiBuffer, "@", keyMiTemp);
	}

	//����һ���ַ����� ���ռ���id ���ܺ����Ϣ ���ܺ��key ���� uid#key#fromid#msg����
	char addBuffer[FILEBUFFER] = { 0 };
	//uid;msg;from;key

	sprintf(addBuffer, "%s#%s#%s#%s#\n", reciver, keyMsg, logonUser.uid, keyMiBuffer);

	//����addBuffer ��\����\\

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

//�������32���ַ���Ϊkey
void GetRand8Char(char *a)
{
	srand((unsigned)time(NULL)); //��ʱ�����֣�ÿ�β����������һ��
	int index = 0;
	while (index < KEYLENTH)
	{
		//������� 32λ��д�ַ�
		char c = rand() % 25 + 65;
		a[index++] = c;
	}
}
