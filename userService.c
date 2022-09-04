#pragma once

#include <string.h>
#include "userService.h"


char userFilePath[FILEBUFFER];
FILE* userFile = NULL;

char privKeyFilePath[FILEBUFFER];
FILE* privKeyFile = NULL;

char publKeyFilePath[FILEBUFFER];
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
	char privKeyBuffer[FILEBUFFER];
	privKeyFile = fopen(privKeyFilePath, "r");

	while (fgets(privKeyBuffer, FILEBUFFER, privKeyFile) != NULL)
	{
		if (privKeyBuffer[0] == '\n') continue;

		int i = 0;
		char uid[8] = { 0 };

		for (; privKeyBuffer[i] != ';'; i++)
			uid[i] = privKeyBuffer[i];

		if (strcmp(uid, logonUser.uid))
			continue;

		//��ȡd
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

//�鹫Կ
int GetPublicKeyByUserIdApi(char uid[], int* e, int* n)
{
	char publicKeyBuffer[FILEBUFFER];
	publKeyFile = fopen(publKeyFilePath, "r");

	while (fgets(publicKeyBuffer, FILEBUFFER, publKeyFile) != NULL)
	{
		if (publicKeyBuffer[0] == '\n') continue;

		int i = 0;
		char id[9] = { 0 };

		for (; publicKeyBuffer[i] != ';'; i++)
			id[i] = publicKeyBuffer[i];

		if (strcmp(id, uid))
			continue;

		//��ȡe
		char ce[8] = { 0 };
		char cn[8] = { 0 };

		i++;
		for (int j = 0; publicKeyBuffer[i] != ';'; i++, j++)
			ce[j] = publicKeyBuffer[i];

		i++;
		for (int j = 0; publicKeyBuffer[i] != ';'; i++, j++)
			cn[j] = publicKeyBuffer[i];
		*e = atoi(ce);
		*n = atoi(cn);

		return 1;
	}
	return 0;
}


//��ȡ���е���ϵ�� 
void GetUserListApi(MailUserLink* userList)
{
	int no = 0;
	char userPath[FILEPATH];
	char userBuffer[FILEBUFFER];

	MailUserLink* p = userList;

	sprintf(userPath, "%s\\data\\user.txt", AppPath);

	userFile = fopen(userPath, "r");

	if (userFile == NULL)
	{
		printf("%s �ļ������ڣ�����", userPath);
		return;
	}


	while (fgets(userBuffer, FILEBUFFER, userFile) != NULL)
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
		char pubKeyBuffer[FILEBUFFER];
		char pubFilePath[FILEPATH];
		sprintf(pubFilePath, "%s\\data\\publickey.txt", AppPath);
		FILE* pubKeyFile = fopen(pubFilePath, "r");

		if (pubKeyFile == NULL)
		{
			printf("%s �ļ�·�������ڣ�����", pubFilePath);
			return;
		}

		while (fgets(pubKeyBuffer, FILEBUFFER, pubKeyFile) != NULL)
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

			node->user.e = atoi(ce);
			node->user.n = atoi(cn);
			break;
		}

		fclose(pubKeyFile);

		strcpy(node->user.uid, uid);
		node->user.no = ++no;

		p->next = node;
		p = p->next;
	}
	fclose(userFile);
}
