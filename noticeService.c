#pragma once

#include "md5.h"
#include "MD5_.h"
#include "pojo.h"
#include "rsa.h"
#include "noticeService.h"

char noticeFilePath[FILEPATH] = { 0 };
FILE* noticeFile = NULL;

int NoticeInitApi()
{
	sprintf(noticeFilePath, "%s\\data\\notices.txt",AppPath);
	noticeFile = fopen(noticeFilePath, "r");

	if (noticeFile == NULL)
	{
		printf("%s ϵͳ�ļ������ڣ������ļ�·��\n", noticeFilePath);
		return 0;
	}
	else
		return 1;
}

int SendNoticeApi(char notice[], RsaPriKey privKey)
{
	//��ȡ��ǰ��ʱ��
	char sendTime[TIMELENTH] = { 0 };
	time_t timep;
	time(&timep);
	struct tm* p;
	p = gmtime(&timep);
	sprintf(sendTime, "%d-%d-%d %d : %d : %d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);

	//��������MD5����ժҪ
	unsigned char MD5[33] = { 0 };
	md5(notice, strlen(notice), MD5);

	//��ժҪ����
	unsigned char keyMd5[64] = { 0 };
	RsaDecipher(MD5, strlen(MD5), keyMd5, privKey);

	//��� buffer д���ļ�
	char addBuffer[FILEBUFFER] = { 0 };
	sprintf(addBuffer, "%s#%s#%s#%s#%s#\n", logonUser.uid, sendTime, notice, MD5, keyMd5);

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

	noticeFile = fopen(noticeFilePath, "a+");

	int add = fputs(addBuffer, noticeFile);

	fclose(noticeFile);

	printf("\n\t��Ϊ��������ժҪ->:%s\n", MD5);

	system("pause");

	if (add == EOF)
		return 0;
	else
		return 1;
}

int GetNoticeListApi(NoticeLink* head)
{
	int no = 0;
	noticeFile = fopen(noticeFilePath, "r");
	char noticeBuffer[FILEBUFFER] = { 0 };
	NoticeLink* p = head;

	while (fgets(noticeBuffer, FILEBUFFER,noticeFile) != NULL)
	{
		int i = 0;
		//ȥ������
		if (noticeBuffer[0] == '\n') continue;

		char uid[USERIDLENTH] = { 0 };
		for (; noticeBuffer[i] != '#'; i++)
			uid[i] = noticeBuffer[i];

		i++;
		char sendTime[TIMELENTH] = { 0 };
		for (int j = 0; noticeBuffer[i] != '#'; i++,j++)
			sendTime[j] = noticeBuffer[i];

		i++;
		char notice[NOTICELENTH] = { 0 };
		for (int j = 0; noticeBuffer[i] != '#'; i++, j++)
			notice[j] = noticeBuffer[i];

		i++;
		char MD5[NOTICELENTH] = { 0 };
		for (int j = 0; noticeBuffer[i] != '#'; i++, j++)
			MD5[j] = noticeBuffer[i];

		i++;
		char keyMd5[NOTICELENTH] = { 0 };
		for (int j = 0; noticeBuffer[i] != '#'; i++, j++)
		{
			if (noticeBuffer[i] == '\n') break;
			keyMd5[j] = noticeBuffer[i];
		}
			

		NoticeLink* node = (NoticeLink*)malloc(sizeof(NoticeLink));

		node->notice.no = ++no;
		node->next = NULL;
		strcpy(node->notice.uid, uid);
		strcpy(node->notice.publishTiem, sendTime);
		strcpy(node->notice.notice, notice);
		strcpy(node->notice.keyMd5, keyMd5);
		strcpy(node->notice.MD5, MD5);

		p->next = node;
		p = p->next;
	}
	fclose(noticeFile);
	return 1;
}
