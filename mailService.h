#pragma once

#include "pojo.h"
#include "des.h"
#include "rsa.h"


//˽�ſ��Ʋ��ʼ��
int MailInitApi();

void GetRand8Char(char* a);

//��ȡȫ����˽�ţ��������δ����
void GetLetter();

//��ȡ��½�ߵ�˽�ţ���Ҫ����һ�������ͷ�ڵ�
void GetMailListApi(MailLink *mainList);

//��ȡ����ϵ��
void GetMialUserListApi(MailUserLink* userList);


//����˽��
/**
 * ������Ϣ��Ҫһ���ռ��ˣ��ͷ��͵�������Ϣ,�ټ�һ���ռ��˵Ĺ�Կ.
 * ���������߻�ȡ��  �����Ǳ��ܵõ�����ֱ�Ӵ�������
 */
int SendMailApi(char reciver[], char msg[], RsaPubKey pubKey);




