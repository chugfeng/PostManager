#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "pojo.h"


int UserInitApi();

//ͨ��ָ�����û���ѯ�û���˽Կ
int GetPriveKeyByUserIdApi(char uid[], int* d, int* n);
