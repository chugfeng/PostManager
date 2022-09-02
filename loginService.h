#pragma once

#include "rsa.h"
#include "pojo.h"

/*****************************************************************//**
 * \file   login.h
 * \brief  
 * 登录和注册的api
 * \author Lenovo
 * \date   August 2022
 *********************************************************************/


/**
 * 登录的API.
 * \param id 登录的id
 * \param pass 登录的密码
 * \param re 登录成功后获取的公钥的e
 * \param rn 登录成功后获取的公钥的n
 * \return 1登录成功 0登录失败
 */
int LoginApi(char id[],char pass[],int *re,int *rn);

int RegistApi(char id[], char pass[]);

//判断是否注册过
int CheckUidApi(char id[]);
