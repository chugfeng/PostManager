#pragma once


#include <time.h>
#include <stdio.h>
#include "md5.h"
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
		printf("%s 系统文件不存在，请检查文件路径\n", noticeFilePath);
		return 0;
	}
	else
		return 1;
}

int SendNoticeApi(char notice[], RsaPriKey privKey)
{
	//获取当前的时间
	char sendTime[TIMELENTH] = { 0 };
	time_t timep;
	time(&timep);
	struct tm* p;
	p = gmtime(&timep);
	sprintf(sendTime, "%d-%d-%d %d : %d : %d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);

	//将公告用MD5生成摘要
	char MD5[16] = { 0 };
	md5(notice, strlen(notice), MD5);

	//将摘要格式化
	char temp[8] = { 0 };
	char decrypt32[64] = { 0 };
	char md5216[NOTICELENTH] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		sprintf(temp, "%0X", MD5[i]);
		strcat((char*)decrypt32, temp);
	}

	printf("公告摘要 -> %s", decrypt32);

	//将摘要加密
	char keyMd5[64];
	RsaDecipher(decrypt32, strlen(decrypt32), keyMd5, privKey);

	//组成 buffer 写入文件
	char addBuffer[FILEBUFFER] = { 0 };
	sprintf(addBuffer, "%s#%s#%s#\n", logonUser.uid, sendTime, keyMd5);

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

	noticeFile = fopen(noticeFilePath, "a+");

	int add = fputs(addBuffer, noticeFile);

	fclose(noticeFile);

	if (add == EOF)
		return 0;
	else
		return 1;
}
