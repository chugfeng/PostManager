#pragma once



#include "pojo.h"
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

int SendNoticeApi(char notice[])
{
	return 0;
}
