#pragma once

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdio.h>

int NoticeInitApi();

//���͹���
int SendNoticeApi(char notice[], RsaPriKey privKey);

//��ȡ����
int GetNoticeListApi(NoticeLink* head);
