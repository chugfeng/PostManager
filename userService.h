#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "pojo.h"


int UserInitApi();

//通过指定的用户查询用户的私钥
int GetPriveKeyByUserIdApi(char uid[], int* d, int* n);

//通过指定用户查询用户的公钥
int GetPublicKeyByUserIdApi(char uid[], int* e, int* n);

//获取所有的人员
void GetUserListApi(MailUserLink* userList);
