#pragma once

#include <string.h>
#include "userService.h"


char userFilePath[MAXPATH];
FILE* userFile = NULL;

char privKeyFilePath[MAXPATH];
FILE* privKeyFile = NULL;

char publKeyFilePath[MAXPATH];
FILE* publKeyFile = NULL;

int UserInitApi()
{
	sprintf(userFilePath, "%s\\data\\mails.txt", AppPath);
	userFile = fopen(userFilePath, "r");
	if (userFile == NULL)
	{
		printf("ϵͳȱʧ��Ҫ�ļ������飬�ļ�·��->%s", userFilePath);
		return 0;
	}
	fclose(userFile);

	sprintf(privKeyFilePath, "%s\\data\\privatekey.txt", AppPath);
	privKeyFile = fopen(privKeyFilePath, "r");
	if (privKeyFile == NULL)
	{
		printf("ϵͳȱʧ��Ҫ�ļ������飬�ļ�·��->%s", privKeyFilePath);
		return 0;
	}
	fclose(privKeyFile);

	sprintf(publKeyFilePath, "%s\\data\\publickey.txt", AppPath);
	publKeyFile = fopen(publKeyFilePath, "r");
	if (publKeyFile == NULL)
	{
		printf("ϵͳȱʧ��Ҫ�ļ������飬�ļ�·��->%s", publKeyFilePath);
		return 0;
	}
	fclose(publKeyFile);
	return 1;

}

//��˽Կ
int GetPriveKeyByUserIdApi(char uid[], int* d, int* n)
{
	char privKeyBuffer[MAXFILEBUFFER];
	privKeyFile = fopen(privKeyFilePath, "r");

	while (fgets(privKeyBuffer, MAXFILEBUFFER, privKeyFile) != NULL)
	{
		if (privKeyBuffer[0] == '\n') continue;

		int i = 0;
		char uid[8] = { 0 };

		for (; privKeyBuffer[i] != ';'; i++)
			uid[i] = privKeyBuffer[i];

		if (strcmp(uid, logonUser.uid))
			continue;

		//��ȡe
		char cd[8] = { 0 };
		char cn[8] = { 0 };

		i++;
		for (int j = 0; privKeyBuffer[i] != ';'; i++, j++)
			cd[j] = privKeyBuffer[i];

		i++;
		for (int j = 0; privKeyBuffer[i] != ';'; i++, j++)
			cn[j] = privKeyBuffer[i];
		*d = atoi(cd);
		*n = atoi(cn);

		return 1;
	}
	return 0;
}

//��ȡ���е���ϵ�� 
void GetMialUserListApi(MailUserLink* userList)
{
	int no = 0;
	char userPath[MAXPATH];
	char userBuffer[MAXFILEBUFFER];

	MailUserLink* p = userList;

	sprintf(userPath, "%s\\data\\user.txt", AppPath);

	userFile = fopen(userPath, "r");

	if (userFile == NULL)
	{
		printf("%s �ļ������ڣ�����", userPath);
		return;
	}


	while (fgets(userBuffer, MAXFILEBUFFER, userFile) != NULL)
	{

		//�ų��������
		if (userBuffer[0] == '\n') continue;

		char uid[8] = { 0 };

		MailUserLink* node = (MailUserLink*)malloc(sizeof(MailUserLink));
		node->next = NULL;
		//����ֻ�ǻ�ȡһ��uid  ���ڿ��ܻ��������
		for (int i = 0; userBuffer[i] != ';'; i++)
			uid[i] = userBuffer[i];

		if (!strcmp(uid, logonUser.uid))
			continue;

		int e = -1, n = -1;
		//get the user plubic Key
		char ce[8] = { 0 };
		char cn[8] = { 0 };
		char pubKeyBuffer[MAXFILEBUFFER];
		char pubFilePath[MAXPATH];
		sprintf(pubFilePath, "%s\\data\\publickey.txt", AppPath);
		FILE* pubKeyFile = fopen(pubFilePath, "r");

		if (pubKeyFile == NULL)
		{
			printf("%s �ļ�·�������ڣ�����", pubFilePath);
			return;
		}

		while (fgets(pubKeyBuffer, MAXFILEBUFFER, pubKeyFile) != NULL)
		{
			char pubKeyUserId[12] = { 0 };
			//�ų��������
			if (pubKeyBuffer[0] == '\n') continue;

			//�ӹ�Կ�����ȡ�ռ��˵Ĺ�Կ
			int i;
			for (i = 0; pubKeyBuffer[i] != ';'; i++)
				pubKeyUserId[i] = pubKeyBuffer[i];

			//���Ϊ�� ˵����һ������Ҫ����
			if (strcmp(pubKeyUserId, uid))
				continue;
			i++;
			for (int j = 0; pubKeyBuffer[i] != ';'; i++, j++)
				ce[j] = pubKeyBuffer[i];
			i++;
			for (int j = 0; pubKeyBuffer[i] != ';'; i++, j++)
				cn[j] = pubKeyBuffer[i];

			p->user.e = atoi(ce);
			p->user.n = atoi(cn);
		}

		fclose(pubKeyFile);

		strcpy(node->user.uid, uid);
		node->user.no = ++no;

		p->next = node;
		p = p->next;
	}
	fclose(userFile);
}
