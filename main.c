#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "pojo.h"
#include "rsa.h"
#include "utils.h"
#include "userService.h"
#include "mailService.h"
#include "loginService.h"
#include "noticeService.h"
#include "MD5_.h"

RsaPubKey publicKey;
RsaPriKey privateKey;

char end[1024] = "感谢使用本系统，请给五心好评哦~~~ ,下次再见！！\n\n\n";

void AppInit();     //启动初始化
void LoginAndResist();
int Login();
void Regist();
int GetMenue();
void ReadMail();
void SendMail();
void PublishNotice();
void ReadNotice();

int main()
{
    AppInit();
    while(1)
    {
        //判断用户是否登录
        if(!strcmp(logonUser.uid,"-1"))
            LoginAndResist();
        
        int menuIndex = GetMenue();

        switch (menuIndex)
        {
        case 0:
            strcpy(logonUser.uid, "-1");break;
        case 1:
            SendMail(); break;
        case 2:
            ReadMail(); break;
        case 3:
            PublishNotice(); break;
        case 4:
            ReadNotice(); break;
        default:
            continue; break;
        }
    }
}

//系统初始化
void AppInit()
{
    strcpy(logonUser.uid, "-1");
	_getcwd(AppPath, FILEPATH);
    if ((MailInitApi() && UserInitApi() && NoticeInitApi()) == 0)
    {
        Sleep(1500);
        exit(0);
    }
        
}

//注册登录界面
void LoginAndResist()
{
    int index;
    int loginFlag = 0;
    while(1)
    {
        if (loginFlag == 1) break;
        system("cls");
        printf("\n\n\n");
        printf("\t\t\t********************欢迎使用JC信箱**********************\n");
        printf("\n\n\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\t\t\t\t\t***    菜单        ***\n");
        printf("\t\t\t\t\t***   1-用户注册   ***\n");
        printf("\t\t\t\t\t***   2-用户登录   ***\n");
        printf("\t\t\t\t\t***   0-退出       ***\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\n\n\n");
        printf("请选择菜单：");

        fflush(stdin);
        int result = scanf("%d",&index);
        if(result == 0)
        {
            printf("请输入数字:\n");
            fflush(stdin);
            Sleep(3000);
            continue;
        }
        else
        {
            switch(index)
            {
            case 1:
                Regist();
                break;
            case 2:
                loginFlag = Login();
                break;
            case 0:
                printf("\n\n\t\t\t%s", end);
                exit(0);
                break;
            default:
                printf("请输入范围内的数字!");
                Sleep(3000);
                continue;
            }
        }
    }
}

//获取菜单
int GetMenue()
{
    int index = 0;
    while (1)
    {
        system("cls");
        printf("\n\n\n");
        printf("\t\t\t********************欢迎使用JC信箱**********************\n");
        printf("\n\n\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\t\t\t\t\t***    菜单        ***\n");
        printf("\t\t\t\t\t***   1-发送私信   ***\n");
        printf("\t\t\t\t\t***   2-查看私信   ***\n");
        printf("\t\t\t\t\t***   3-发送公告   ***\n");
        printf("\t\t\t\t\t***   4-浏览公告   ***\n");
        printf("\t\t\t\t\t***   0-注销登录   ***\n");
        printf("\t\t\t\t\t**********************\n");
        printf("请输入菜单选项：");

        fflush(stdin);
        int result = scanf("%d", &index);

        if (result == 0)
        {
            printf("输入错误！请重新输入\n");
            Sleep(2000);
            continue;
        }
        if (index < 0 || index > 5)
        {
            printf("输入错误！请输入菜单中的选项\n");
            Sleep(2000);
            continue;
        }
        break;
    }
    return index;
}

//登录
int Login()
{
    int count = 0;
    int ret = 0;
    while(1)
    {
        system("cls");
        char  uidBuffer[128] = {0};
        printf("\n\n\n");
        printf("\t\t\t********************欢迎登录JC信箱**********************\n");
        printf("\n\n\n");
        printf("请输入你的账号(输入esc退出)：");
        scanf("%s",uidBuffer);

        if (!strcmp(uidBuffer, "esc")) return;

        char passBuffer[128] = {0};
        printf("请输入你的密码(输入esc退出)：");
        scanf("%s",passBuffer);

        if (!strcmp(passBuffer, "esc")) return;

        int e,n;

        int result = LoginApi(uidBuffer, passBuffer, &e, &n);

        if(result == 1)
        {
            strcpy(logonUser.uid,uidBuffer);

            int d, n;
            int result = GetPriveKeyByUserIdApi(uidBuffer, &d, &n);
            privateKey.d = d;
            privateKey.n = n;

            if (result == 0)
            {
                printf("登录失败!  \t未获取到有效的私钥，请联系管理员\n");
                break;
            }
            ret = 1;
            printf("登录成功!  \t两秒后自动进入系统\n");
            Sleep(2000);
            break;
        }
        else
        {
            count ++;
            if(count >= 3)
            {
                printf("三次登录错误，将重新进入系统\n");
                break;
            }
            printf("登录失败，请重试\n");
            Sleep(2000);
        }
    }
    return ret;
}

//注册
void Regist()
{
    system("cls");
    char  uidBuffer[128] = { 0 };
    char passBuffer[128] = { 0 };
    printf("\n\n\n");
    printf("\t\t\t********************欢迎注册JC信箱**********************\n");
    printf("\n\n\n");

    while (1)
    {
        fflush(stdin);
        printf("\n请输入你的账号（不大于8位）(输入esc取消注册)：");
        scanf("%s", uidBuffer);

        if (!strcmp(uidBuffer, "esc")) return;

        if (strlen(uidBuffer) > 8)
        {
            printf("\t你输入的ID超过8位，请重新输入\n");
            Sleep(2000);
            continue;
        }

        if (CheckUidApi(uidBuffer))
        {
            printf("\t你输入的ID已被注册，请重新输入\n");
            Sleep(2000);
            continue;
        }
        break;
    }
    
    while (1)
    {
        fflush(stdin);
        printf("\n请输入你的密码（不大于16位）(输入esc取消注册)：");
        scanf("%s", passBuffer);

        if (!strcmp(passBuffer, "esc")) return;

        if (strlen(uidBuffer) > 16)
        {
            printf("\t你输入的密码超过16位，请重新输入\n");
            Sleep(2000);
            continue;
        }
        break;
    }

    int e = 0, d = 0, T = 0;
    int resule = RegistApi(uidBuffer, passBuffer, &e, &d, &T);
    
    if (resule == 1)
    {
        publicKey.e = e;
        publicKey.n = T;
        privateKey.d = d;
        privateKey.n = T;
        printf("\t\t注册成功!  \n");
    }
        
    else
        printf("\t\t注册失败！\n");

    Sleep(2000);
}

//发送私信
void SendMail()
{
    system("cls");
    int index = 0;
    int reciveNo;
    int userNum = 0;
    char reciverUser[16] = { 0 };
    char sendMsgBuffer[MSGLENTH];
    printf("\n\n\n");
    printf("\t\t\t********************欢迎进入JC信息界面**********************\n");
    printf("\n\n\n");
    printf("请输入你要发送信息:\n");
    printf("\n");
    fflush(stdin);
    //gets_s(sendMsgBuffer, 1024);
    //TODO这里需要改一下
    scanf("%s", sendMsgBuffer);

    MailUserLink* userList = (MailUserLink*)malloc(sizeof(MailUserLink));
    //userList->user.no = 0;
    MailUserLink* p = userList;
    GetUserListApi(userList);
    
    //展示联系人
    if (p->next != NULL)
    {
        p = p->next;
        printf("\n");
        printf("序号\t\t联系人ID\n");
    }
    else
    {
        printf("\n");
        printf("抱歉 你没有好友\n");
        return;
    }
    

    while (p != NULL)
    {
        userNum++;
        printf("\n");
        printf("%d\t\t%s\n", p->user.no, p->user.uid);
        p = p->next;
    }

    while (1)
    {
        fflush(stdin);
        printf("\n");
        printf("请选择你要发送的联系人:");
        int result = scanf("%d", &reciveNo);

        if (result == 0)
        {
            printf("\n");
            printf("请输入数字!!\n");
            continue;
        }

        if (reciveNo > userNum)
        {
            printf("\n");
            printf("请重新输入，无该用户!!\n");
            continue;
        }
        break;
    }
    p = userList;
    if (p->next != NULL) p = p->next;
    
    RsaPubKey pubKey;
    int pe, pn;
    while (p != NULL)
    {
        if (p->user.no == reciveNo)
        {
            pe = p->user.e;
            pn = p->user.n;

            strcpy(reciverUser, p->user.uid);
            break;
        }
        p = p->next;
    }
    pubKey.e= pe;
    pubKey.n= pn;
    
    int send = SendMailApi(reciverUser, sendMsgBuffer, pubKey);

    if (send == 1)
    {
        printf("\n");
        printf("发送成功！咻~~~\n");
    }
    else if (send == -1)
    {
        printf("\n");
        printf("暂时存在了草稿箱\n");
    }
    else if (send == 0)
    {
        printf("\n");
        printf("发送失败，请重试!\n");
    }

    Sleep(1000);
}

//查看私信
void ReadMail()
{
    int index = 0;
    int mailNum = 0;
    //获取到私信
    MailLink* head = (MailLink*)malloc(sizeof(MailLink));
    head->next = NULL;
    GetMailListApi(head);
    MailLink* p = head;
    //展示私信
    
    if (p->next == NULL)
    {
        printf("伙计，你的邮箱是空的，请多和小伙伴交流\n");
        Sleep(2000);
        return;
    }
    p = p->next;

    system("cls");
    printf("序号\t\t发送者ID\n");
    while (p != NULL)
    {
        printf("%d\t\t%s\n", p->mail.no, p->mail.fromUId);
        mailNum++;
        p = p->next;
    }
    while (1)
    {
        printf("请输入你想查看的私信序号:");
        int result = scanf("%d", &index);

        if (result == EOF || result == 0 || result <0 || index > mailNum)
        {
            printf("嘿老朋友，你要是再这样，小心我揍你的屁股");
            Sleep(3000);
        }
        break;
    }

    p = head->next;

    while (1)
    {
        if (p->mail.no == index)
            break;
        p = p->next;
    }

    /**
     * 开始解密.
     */

    //通过自己的私钥解开加密的DES密钥
    
    long keyBuffer[256] = { 0 };
    char keyTemp[8] = { 0 };
    index = 0;
    int ind = 0;
    int longInde = 0;
    //****@****@******@*****
    while (1)
    {
        char c = p->mail.key[ind++];

        if (c == '@' || c == '\0')
        {
            keyBuffer[longInde++] = atoi(keyTemp);
            index = 0;

            memcpy(keyTemp, "0", 8);
            if (c == '\0') break;
        }
        else
            keyTemp[index++] = c;
    }

    //解密密钥
    char key[KEYLENTH] = { 0 };
    RsaDecipher(keyBuffer, KEYLENTH, key, privateKey);

    //解密密文
    char msg[MSGLENTH] = { 0 };
    DES_Decrypt(p->mail.msg, strlen(p->mail.msg), key, msg);

    system("cls");
    printf("你收到一份来自%s的邮件，邮件内容如下:\n", p->mail.fromUId);
    printf("\t%s\n", msg);

    system("pause");
}

//发布公告
void PublishNotice()
{
    system("cls");
    printf("请输入公告:\n");
    char notice[NOTICELENTH] = { 0 };

    //TODO这里需要改一下
    //gets(notice, NOTICELENTH);
    scanf("%s", notice);

    int send = SendNoticeApi(notice, privateKey);

    if (send == 1)
    {
        printf("\n");
        printf("发布成功！\n");
    }
    else if (send == 0)
    {
        printf("\n");
        printf("发布失败，请重试!\n");
    }

    Sleep(1000);
}

//查看公告
void ReadNotice()
{
    NoticeLink* head = (NoticeLink*)malloc(sizeof(NoticeLink));
    head->next = NULL;
    GetNoticeListApi(head);

    if (head->next == NULL)
    {
        printf("暂时没有公告！！");
        Sleep(2000);
        return;
    }

    NoticeLink* p = head->next;
    printf("序号\t\t发布者ID\t\t发布时间\t\t摘要\n");

    int noticeNum = 0;
    while (p != NULL)
    {
        noticeNum++;
        printf("%d\t\t%s\t\t%s\t\t%s\n", p->notice.no, p->notice.uid, p->notice.publishTiem,p->notice.MD5);
        p = p->next;
    }

    while (1)
    {
        printf("\n\n");
        printf("请选择你要查看的公告，并校验公告！(-1退出)\n请输入序号->:");
        int index = 0;

        while (1)
        {
            int result = scanf("%d", &index);

            if (result == EOF || result == 0 || result <0 || index > noticeNum)
            {
                printf("嘿老朋友，你要是再这样，小心我揍你的屁股");
                Sleep(3000);
            }
            break;
        }

        if (index == -1) break;

        p = head->next;

        while (1)
        {
            if (p->notice.no == index)
                break;
            p = p->next;
        }

        char MD5[33] = { 0 };
        md5(p->notice.notice, strlen(p->notice.notice), MD5);

        int e, n;
        int ret = GetPublicKeyByUserIdApi(p->notice.uid, &e, &n);

        if (ret == 0)
        {
            printf("未获取到指定用户的公钥，校验失败\n");
            Sleep(2000);
            return;
        }

        RsaPubKey pukey;
        pukey.e = e;
        pukey.n = n;

        char deKeyMd5[65] = { 0 };
        RsaEncipher(p->notice.keyMd5, strlen(p->notice.keyMd5), deKeyMd5, pukey);

        if (strcmp(MD5, p->notice.MD5))
        {
            printf("校验失败，无法查看公告\n");
            Sleep(2000);
            return;
        }

        system("cls");
        printf("发布人ID：%s\n\n", p->notice.uid);
        printf("发布时间：%s\n\n", p->notice.publishTiem);

        printf("发布内容：%s\n\n", p->notice.notice);

        system("pause");
    }
}