#pragma once

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdio.h>

int NoticeInitApi();

//发送公告
int SendNoticeApi(char notice[], RsaPriKey privKey);

//获取公告
int GetNoticeListApi(NoticeLink* head);
