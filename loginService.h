#pragma once

#include "rsa.h"
#include "pojo.h"

/*****************************************************************//**
 * \file   login.h
 * \brief  
 * ��¼��ע���api
 * \author Lenovo
 * \date   August 2022
 *********************************************************************/


/**
 * ��¼��API.
 * \param id ��¼��id
 * \param pass ��¼������
 * \param re ��¼�ɹ����ȡ�Ĺ�Կ��e
 * \param rn ��¼�ɹ����ȡ�Ĺ�Կ��n
 * \return 1��¼�ɹ� 0��¼ʧ��
 */
int LoginApi(char id[],char pass[],int *re,int *rn);

int RegistApi(char id[], char pass[]);

//�ж��Ƿ�ע���
int CheckUidApi(char id[]);
