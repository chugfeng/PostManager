#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "pojo.h"


int UserInitApi();

//ͨ��ָ�����û���ѯ�û���˽Կ
int GetPriveKeyByUserIdApi(char uid[], int* d, int* n);

//ͨ��ָ���û���ѯ�û��Ĺ�Կ
int GetPublicKeyByUserIdApi(char uid[], int* e, int* n);

//��ȡ���е���Ա
void GetUserListApi(MailUserLink* userList);
