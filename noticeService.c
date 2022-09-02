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
		printf("%s 系统文件不存在，请检查文件路径\n", noticeFilePath);
		return 0;
	}
	else
		return 1;
}

int SendNoticeApi(char notice[])
{
	return 0;
}
