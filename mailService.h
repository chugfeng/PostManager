#pragma once

#include "pojo.h"
#include "des.h"
#include "rsa.h"


//私信控制层初始化
int MailInitApi();

void GetRand8Char(char* a);

//读取全部的私信，这个功能未开发
void GetLetter();

//读取登陆者的私信，需要传入一个链表的头节点
void GetMailListApi(MailLink *mainList);

//获取到联系人
void GetMialUserListApi(MailUserLink* userList);


//发送私信
/**
 * 发送消息需要一个收件人，和发送的明文信息,再加一个收件人的公钥.
 * 本来想从这边获取的  后来那边能得到，就直接传过来了
 */
int SendMailApi(char reciver[], char msg[], RsaPubKey pubKey);




