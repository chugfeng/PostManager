#pragma once

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

int NoticeInitApi();

//���͹���
int SendNoticeApi(char notice[], RsaPriKey privKey);

//��ȡ����
int GetNoticeListApi(NoticeLink* head);
