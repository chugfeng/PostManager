#pragma once

#include <stdio.h>

int NoticeInitApi();

//���͹���
int SendNoticeApi(char notice[], RsaPriKey privKey);
