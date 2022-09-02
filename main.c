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

RsaPubKey publicKey;
RsaPriKey privateKey;

int systemExit = 0;
char end[1024] = "感谢使用本系统，请给五心好评哦~~~ ,下次再见！！\n\n\n";

int e = 0, d = 0, T = 0;

void AppInit();     //启动初始化
void LoginAndResist();
int Login();
void Regist();
int GetMenue();
void ReadMail();
void SendMail();
void PublishNotice();

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
        default:
            continue; break;
        }
    }
}

void AppInit()
{
    strcpy(logonUser.uid, "-1");
	_getcwd(AppPath, MAXPATH);
    if ((MailInitApi() && UserInitApi() && NoticeInitApi()) == 0)
    {
        Sleep(1500);
        exit(0);
    }
        
}

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
        if (index < 0 || index > 3)
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
        char  uidBuffer[MAXUSERID] = {0};
        printf("\n\n\n");
        printf("\t\t\t********************欢迎登录JC信箱**********************\n");
        printf("\n\n\n");
        printf("请输入你的账号：");
        scanf("%s",uidBuffer);

        char passBuffer[FILEPATH] = {0};
        printf("请输入你的密码：");
        scanf("%s",passBuffer);

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
    char  uidBuffer[MAXUSERID] = {0};
    printf("\n\n\n");
    printf("\t\t\t********************欢迎注册JC信箱**********************\n");
    printf("\n\n\n");
    printf("请输入你的账号（不大于8位）：");
    scanf("%s",uidBuffer);

    char passBuffer[FILEPATH] = {0};
    printf("请输入你的密码（不大于16位）：");
    scanf("%s",passBuffer);

    char uid[8] = {0};
    char pass[16] = {0};

    //获取到user文件的路径
    char userpath[MAXPATH];
    sprintf(userpath, "%s\\data\\user.txt", AppPath);

    FILE *file = fopen(userpath, "a+");
    if (file == NULL)
	{
		printf("%s 文件不存在，请检查\n", userpath);
		Sleep(3000);
	}

    //获取到随机密钥
    
    //GetKey(&e, &d, &T);
    GetKey();
    publicKey.e = e;
    publicKey.n = T;
    privateKey.d = d;
    privateKey.n = T;

    //把公钥保存到文件中
    strcpy(uid,uidBuffer);
    char pubKeyPath [MAXPATH] = {0};
    sprintf(pubKeyPath,"%s\\data\\publickey.txt",AppPath);

    FILE *pkFile = fopen(pubKeyPath,"a+");

    char pkBuffer[1024] = {0};
    sprintf(pkBuffer,"%s;%d;%d;\n",uid,e,T);
    fputs(pkBuffer,pkFile);
    fclose(pkFile);

    //把私钥保存到文件中

    char priKeyPath [MAXPATH] = {0};
    sprintf(priKeyPath,"%s\\data\\privatekey.txt",AppPath);

    FILE *pvFile = fopen(priKeyPath,"a+");

    char pvBuffer[1024] = {0};
    sprintf(pvBuffer,"%s;%d;%d;\n",uid,d,T);
    fputs(pvBuffer,pvFile);
    fclose(pvFile);

    strcpy(uid,uidBuffer);
    strcpy(pass, passBuffer);


    char _buffer[48] = {0};
    sprintf(_buffer, "%s;%s;\n", uid,pass);

    //将账号密码保存起来
    int result  = fputs(_buffer,file);
    if(result == 0)
        printf("注册成功!  \n账号密码为%s",_buffer);
    else
        printf("注册失败！\n");

    Sleep(3000);
    fclose(file);
}

//获取密钥
int GetKey()
{
    int p, q, n, phi;
    srand(time(NULL));
    while (1) {
        p = randPrime(SINGLE_MAX);

        q = randPrime(SINGLE_MAX);

        n = p * q;

        if (n >= 128)
            break;
    }

    phi = (p - 1) * (+-q - 1);

    e = randExponent(phi, EXPONENT_MAX);

    d = inverse(e, phi);

    T = n;




    e = 173;d = 4037;T = 13971;
    return 1;
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
    //strcpy(sendMsgBuffer, "aabccdd");
    scanf("%s", sendMsgBuffer);

    MailUserLink* userList = (MailUserLink*)malloc(sizeof(MailUserLink));
    //userList->user.no = 0;
    MailUserLink* p = userList;
    GetMialUserListApi(userList);
    
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

        if (result == EOF || result == 0 || result <0 || result > mailNum)
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
    printf("请输入公告:\n");
    char notice[NOTICELENTH] = { 0 };

    //TODO这里需要改一下
    scanf("%s", notice);

    int send = SendNoticeApi(notice);

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
