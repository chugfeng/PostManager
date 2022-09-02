#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "pojo.h"


int UserInitApi();

//通过指定的用户查询用户的私钥
int GetPriveKeyByUserIdApi(char uid[], int* d, int* n);
