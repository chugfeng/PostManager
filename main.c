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
char end[1024] = "��лʹ�ñ�ϵͳ��������ĺ���Ŷ~~~ ,�´��ټ�����\n\n\n";

int e = 0, d = 0, T = 0;

void AppInit();     //������ʼ��
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
        if (index < 0 || index > 3)
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
        char  uidBuffer[MAXUSERID] = {0};
        printf("\n\n\n");
        printf("\t\t\t********************��ӭ��¼JC����**********************\n");
        printf("\n\n\n");
        printf("����������˺ţ�");
        scanf("%s",uidBuffer);

        char passBuffer[FILEPATH] = {0};
        printf("������������룺");
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
    char  uidBuffer[MAXUSERID] = {0};
    printf("\n\n\n");
    printf("\t\t\t********************��ӭע��JC����**********************\n");
    printf("\n\n\n");
    printf("����������˺ţ�������8λ����");
    scanf("%s",uidBuffer);

    char passBuffer[FILEPATH] = {0};
    printf("������������루������16λ����");
    scanf("%s",passBuffer);

    char uid[8] = {0};
    char pass[16] = {0};

    //��ȡ��user�ļ���·��
    char userpath[MAXPATH];
    sprintf(userpath, "%s\\data\\user.txt", AppPath);

    FILE *file = fopen(userpath, "a+");
    if (file == NULL)
	{
		printf("%s �ļ������ڣ�����\n", userpath);
		Sleep(3000);
	}

    //��ȡ�������Կ
    
    //GetKey(&e, &d, &T);
    GetKey();
    publicKey.e = e;
    publicKey.n = T;
    privateKey.d = d;
    privateKey.n = T;

    //�ѹ�Կ���浽�ļ���
    strcpy(uid,uidBuffer);
    char pubKeyPath [MAXPATH] = {0};
    sprintf(pubKeyPath,"%s\\data\\publickey.txt",AppPath);

    FILE *pkFile = fopen(pubKeyPath,"a+");

    char pkBuffer[1024] = {0};
    sprintf(pkBuffer,"%s;%d;%d;\n",uid,e,T);
    fputs(pkBuffer,pkFile);
    fclose(pkFile);

    //��˽Կ���浽�ļ���

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

    //���˺����뱣������
    int result  = fputs(_buffer,file);
    if(result == 0)
        printf("ע��ɹ�!  \n�˺�����Ϊ%s",_buffer);
    else
        printf("ע��ʧ�ܣ�\n");

    Sleep(3000);
    fclose(file);
}

//��ȡ��Կ
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
    //strcpy(sendMsgBuffer, "aabccdd");
    scanf("%s", sendMsgBuffer);

    MailUserLink* userList = (MailUserLink*)malloc(sizeof(MailUserLink));
    //userList->user.no = 0;
    MailUserLink* p = userList;
    GetMialUserListApi(userList);
    
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

        if (result == EOF || result == 0 || result <0 || result > mailNum)
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
    printf("�����빫��:\n");
    char notice[NOTICELENTH] = { 0 };

    //TODO������Ҫ��һ��
    scanf("%s", notice);

    int send = SendNoticeApi(notice);

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
