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

char end[1024] = "��лʹ�ñ�ϵͳ��������ĺ���Ŷ~~~ ,�´��ټ�����\n\n\n";

void AppInit();     //������ʼ��
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
        //�ж��û��Ƿ��¼
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

//ϵͳ��ʼ��
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

//ע���¼����
void LoginAndResist()
{
    int index;
    int loginFlag = 0;
    while(1)
    {
        if (loginFlag == 1) break;
        system("cls");
        printf("\n\n\n");
        printf("\t\t\t********************��ӭʹ��JC����**********************\n");
        printf("\n\n\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\t\t\t\t\t***    �˵�        ***\n");
        printf("\t\t\t\t\t***   1-�û�ע��   ***\n");
        printf("\t\t\t\t\t***   2-�û���¼   ***\n");
        printf("\t\t\t\t\t***   0-�˳�       ***\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\n\n\n");
        printf("��ѡ��˵���");

        fflush(stdin);
        int result = scanf("%d",&index);
        if(result == 0)
        {
            printf("����������:\n");
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
                printf("�����뷶Χ�ڵ�����!");
                Sleep(3000);
                continue;
            }
        }
    }
}

//��ȡ�˵�
int GetMenue()
{
    int index = 0;
    while (1)
    {
        system("cls");
        printf("\n\n\n");
        printf("\t\t\t********************��ӭʹ��JC����**********************\n");
        printf("\n\n\n");
        printf("\t\t\t\t\t**********************\n");
        printf("\t\t\t\t\t***    �˵�        ***\n");
        printf("\t\t\t\t\t***   1-����˽��   ***\n");
        printf("\t\t\t\t\t***   2-�鿴˽��   ***\n");
        printf("\t\t\t\t\t***   3-���͹���   ***\n");
        printf("\t\t\t\t\t***   4-�������   ***\n");
        printf("\t\t\t\t\t***   0-ע����¼   ***\n");
        printf("\t\t\t\t\t**********************\n");
        printf("������˵�ѡ�");

        fflush(stdin);
        int result = scanf("%d", &index);

        if (result == 0)
        {
            printf("�����������������\n");
            Sleep(2000);
            continue;
        }
        if (index < 0 || index > 5)
        {
            printf("�������������˵��е�ѡ��\n");
            Sleep(2000);
            continue;
        }
        break;
    }
    return index;
}

//��¼
int Login()
{
    int count = 0;
    int ret = 0;
    while(1)
    {
        system("cls");
        char  uidBuffer[128] = {0};
        printf("\n\n\n");
        printf("\t\t\t********************��ӭ��¼JC����**********************\n");
        printf("\n\n\n");
        printf("����������˺�(����esc�˳�)��");
        scanf("%s",uidBuffer);

        if (!strcmp(uidBuffer, "esc")) return;

        char passBuffer[128] = {0};
        printf("�������������(����esc�˳�)��");
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
                printf("��¼ʧ��!  \tδ��ȡ����Ч��˽Կ������ϵ����Ա\n");
                break;
            }
            ret = 1;
            printf("��¼�ɹ�!  \t������Զ�����ϵͳ\n");
            Sleep(2000);
            break;
        }
        else
        {
            count ++;
            if(count >= 3)
            {
                printf("���ε�¼���󣬽����½���ϵͳ\n");
                break;
            }
            printf("��¼ʧ�ܣ�������\n");
            Sleep(2000);
        }
    }
    return ret;
}

//ע��
void Regist()
{
    system("cls");
    char  uidBuffer[128] = { 0 };
    char passBuffer[128] = { 0 };
    printf("\n\n\n");
    printf("\t\t\t********************��ӭע��JC����**********************\n");
    printf("\n\n\n");

    while (1)
    {
        fflush(stdin);
        printf("\n����������˺ţ�������8λ��(����escȡ��ע��)��");
        scanf("%s", uidBuffer);

        if (!strcmp(uidBuffer, "esc")) return;

        if (strlen(uidBuffer) > 8)
        {
            printf("\t�������ID����8λ������������\n");
            Sleep(2000);
            continue;
        }

        if (CheckUidApi(uidBuffer))
        {
            printf("\t�������ID�ѱ�ע�ᣬ����������\n");
            Sleep(2000);
            continue;
        }
        break;
    }
    
    while (1)
    {
        fflush(stdin);
        printf("\n������������루������16λ��(����escȡ��ע��)��");
        scanf("%s", passBuffer);

        if (!strcmp(passBuffer, "esc")) return;

        if (strlen(uidBuffer) > 16)
        {
            printf("\t����������볬��16λ������������\n");
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
        printf("\t\tע��ɹ�!  \n");
    }
        
    else
        printf("\t\tע��ʧ�ܣ�\n");

    Sleep(2000);
}

//����˽��
void SendMail()
{
    system("cls");
    int index = 0;
    int reciveNo;
    int userNum = 0;
    char reciverUser[16] = { 0 };
    char sendMsgBuffer[MSGLENTH];
    printf("\n\n\n");
    printf("\t\t\t********************��ӭ����JC��Ϣ����**********************\n");
    printf("\n\n\n");
    printf("��������Ҫ������Ϣ:\n");
    printf("\n");
    fflush(stdin);
    //gets_s(sendMsgBuffer, 1024);
    //TODO������Ҫ��һ��
    scanf("%s", sendMsgBuffer);

    MailUserLink* userList = (MailUserLink*)malloc(sizeof(MailUserLink));
    //userList->user.no = 0;
    MailUserLink* p = userList;
    GetUserListApi(userList);
    
    //չʾ��ϵ��
    if (p->next != NULL)
    {
        p = p->next;
        printf("\n");
        printf("���\t\t��ϵ��ID\n");
    }
    else
    {
        printf("\n");
        printf("��Ǹ ��û�к���\n");
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
        printf("��ѡ����Ҫ���͵���ϵ��:");
        int result = scanf("%d", &reciveNo);

        if (result == 0)
        {
            printf("\n");
            printf("����������!!\n");
            continue;
        }

        if (reciveNo > userNum)
        {
            printf("\n");
            printf("���������룬�޸��û�!!\n");
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
        printf("���ͳɹ�����~~~\n");
    }
    else if (send == -1)
    {
        printf("\n");
        printf("��ʱ�����˲ݸ���\n");
    }
    else if (send == 0)
    {
        printf("\n");
        printf("����ʧ�ܣ�������!\n");
    }

    Sleep(1000);
}

//�鿴˽��
void ReadMail()
{
    int index = 0;
    int mailNum = 0;
    //��ȡ��˽��
    MailLink* head = (MailLink*)malloc(sizeof(MailLink));
    head->next = NULL;
    GetMailListApi(head);
    MailLink* p = head;
    //չʾ˽��
    
    if (p->next == NULL)
    {
        printf("��ƣ���������ǿյģ�����С��齻��\n");
        Sleep(2000);
        return;
    }
    p = p->next;

    system("cls");
    printf("���\t\t������ID\n");
    while (p != NULL)
    {
        printf("%d\t\t%s\n", p->mail.no, p->mail.fromUId);
        mailNum++;
        p = p->next;
    }
    while (1)
    {
        printf("����������鿴��˽�����:");
        int result = scanf("%d", &index);

        if (result == EOF || result == 0 || result <0 || index > mailNum)
        {
            printf("�������ѣ���Ҫ����������С���������ƨ��");
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
     * ��ʼ����.
     */

    //ͨ���Լ���˽Կ�⿪���ܵ�DES��Կ
    
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

    //������Կ
    char key[KEYLENTH] = { 0 };
    RsaDecipher(keyBuffer, KEYLENTH, key, privateKey);

    //��������
    char msg[MSGLENTH] = { 0 };
    DES_Decrypt(p->mail.msg, strlen(p->mail.msg), key, msg);

    system("cls");
    printf("���յ�һ������%s���ʼ����ʼ���������:\n", p->mail.fromUId);
    printf("\t%s\n", msg);

    system("pause");
}

//��������
void PublishNotice()
{
    system("cls");
    printf("�����빫��:\n");
    char notice[NOTICELENTH] = { 0 };

    //TODO������Ҫ��һ��
    //gets(notice, NOTICELENTH);
    scanf("%s", notice);

    int send = SendNoticeApi(notice, privateKey);

    if (send == 1)
    {
        printf("\n");
        printf("�����ɹ���\n");
    }
    else if (send == 0)
    {
        printf("\n");
        printf("����ʧ�ܣ�������!\n");
    }

    Sleep(1000);
}

//�鿴����
void ReadNotice()
{
    NoticeLink* head = (NoticeLink*)malloc(sizeof(NoticeLink));
    head->next = NULL;
    GetNoticeListApi(head);

    if (head->next == NULL)
    {
        printf("��ʱû�й��棡��");
        Sleep(2000);
        return;
    }

    NoticeLink* p = head->next;
    printf("���\t\t������ID\t\t����ʱ��\t\tժҪ\n");

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
        printf("��ѡ����Ҫ�鿴�Ĺ��棬��У�鹫�棡(-1�˳�)\n���������->:");
        int index = 0;

        while (1)
        {
            int result = scanf("%d", &index);

            if (result == EOF || result == 0 || result <0 || index > noticeNum)
            {
                printf("�������ѣ���Ҫ����������С���������ƨ��");
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
            printf("δ��ȡ��ָ���û��Ĺ�Կ��У��ʧ��\n");
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
            printf("У��ʧ�ܣ��޷��鿴����\n");
            Sleep(2000);
            return;
        }

        system("cls");
        printf("������ID��%s\n\n", p->notice.uid);
        printf("����ʱ�䣺%s\n\n", p->notice.publishTiem);

        printf("�������ݣ�%s\n\n", p->notice.notice);

        system("pause");
    }
}