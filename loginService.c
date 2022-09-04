#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "loginService.h"
#include "userService.h"
#include "utils.h"


RsaPriKey pkey;

int GetKey(int* e, int* d, int* T)
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

    *e = randExponent(phi, EXPONENT_MAX);

    *d = inverse(e, phi);

    *T = n;




    *e = 173; *d = 4037; *T = 13971;
    return 1;
}

/**
 * 登录的API.
 * \param id 登录的id
 * \param pass 登录的密码
 * \param re 登录成功后获取的公钥的e
 * \param rn 登录成功后获取的公钥的n
 * \return 1登录成功 0登录失败
 */
int LoginApi(char id[],char pass[],int *re,int *rn)
{
    int pd;
    int pn;
    char path[FILEPATH] = { 0 };
    char buffer[FILEBUFFER] = { 0 };
    int isTrue = 0;

    strcpy(path,AppPath);
    strcat(path,"\\data\\user.txt");

    FILE *file = fopen(path, "r");

    if (file == NULL)
	{
		printf("%s 文件不存在，请检查\n", path);
		return isTrue;
	}

	//按行读取文件
	while (fgets(buffer, FILEBUFFER, file) != NULL)
	{
	    int bufferIndex = 0;
	    //先获取每行的id
	    char uid[8] = {0};
	    for(int i = 0; buffer[i] != ';';i++)
        {
            uid[i] = buffer[i];
            bufferIndex ++;

            if (buffer[i] == '\n') break;
        }
        bufferIndex ++;

        //判断是否和传过来的相等,//为真的时候说明不相等,换下一行
        if(strcmp(uid,id))
           continue;

        //获取到账号后，说明 输入的账户账户注册过，可以从私钥表里获取到这个账户的私钥
        //获取到文件
        int isGetPriKey = 0;
        char priKeyPath [FILEPATH] = {0};
        char pvbuffe[FILEBUFFER]={0};

        sprintf(priKeyPath,"%s\\data\\privatekey.txt",AppPath);
        FILE *pvFile = fopen(priKeyPath,"r");

        if (pvFile == NULL)
        {
            printf("%s 文件不存在，请检查", priKeyPath);
            return 0;
        }

        while (fgets(pvbuffe, FILEBUFFER, pvFile) != NULL)
        {
            char pkuid[16]={0};
            int temp = 0;
            for(int i = 0 ,j = 0;pvbuffe[i] != ';' ; i++,j++)
            {
                pkuid[j] = pvbuffe[i];
                temp++;

                if (pvbuffe[i] == '\n') break;
            }

            //if the uid = user input then get the primary key
            if(!strcmp(pkuid , id))
            {
                //get primary key
                char d[8] = { 0 };
                char n[8] = { 0 };

                //d
                for(int i = temp +1 , j= 0; pvbuffe[i] != ';' ; i++,j++)
                {
                    d[j] = pvbuffe[i];
                    temp++;
                }
                //n
                for(int i = temp +2, j= 0; pvbuffe[i] != ';' ; i++,j++)
                {
                    n[j] = pvbuffe[i];
                    temp++;
                }

                sscanf(d,"%d",&pd);
                sscanf(n,"%d",&pn);
                pkey.d = pd;
                pkey.n = pn;

                isGetPriKey = 1;
                break;
            }
        }
        fclose(pvFile);     //释放指针
        if(isGetPriKey == 0)
        {
            printf("未获取到你的私钥，请联系系统开发人员提交bug\n");
            isTrue = 0;
            return isTrue;
        }

        //账号相等了获取密码
        char pwd[16] = {0};;
        for(int i = bufferIndex ,j = 0; buffer[i] != ';';i++,j++)
            pwd[j] = buffer[i];

        //将获取到的密码解密，正常流程因该是将输入的数据进行加密，这里做测试

        //判断密码是否相等
        if(! strcmp(pwd,pass))
        {
            isTrue = 1;
            break;
        }else
            break;
	}
	return isTrue;
}

int RegistApi(char nid[], char npass[],int *xe,int *xd,int *xn)
{
    char uid[USERIDLENTH] = { 0 };
    char pass[17] = { 0 };

    //获取到随机密钥
    int e = 0, d = 0, T = 0;
    GetKey(&e, &d, &T);

    memcpy(uid, nid, 8);
    //strcpy(uid, nid);
    char pubKeyPath[FILEPATH] = { 0 };
    sprintf(pubKeyPath, "%s\\data\\publickey.txt", AppPath);

    FILE* pkFile = fopen(pubKeyPath, "a+");
    char pkBuffer[1024] = { 0 };
    sprintf(pkBuffer, "%s;%d;%d;\n", uid, e, T);
    fputs(pkBuffer, pkFile);
    fclose(pkFile);

    //把私钥保存到文件中

    char priKeyPath[FILEPATH] = { 0 };
    sprintf(priKeyPath, "%s\\data\\privatekey.txt", AppPath);

    FILE* pvFile = fopen(priKeyPath, "a+");
    char pvBuffer[1024] = { 0 };
    sprintf(pvBuffer, "%s;%d;%d;\n", uid, d, T);
    fputs(pvBuffer, pvFile);
    fclose(pvFile);

    memcpy(pass, npass, 16);
    //strcpy(pass, npass);

    char _buffer[48] = { 0 };
    sprintf(_buffer, "%s;%s;\n", uid, pass);

    //将账号密码保存起来
    char userpath[FILEPATH];
    sprintf(userpath, "%s\\data\\user.txt", AppPath);
    FILE* file = fopen(userpath, "a+");
    int result = fputs(_buffer, file);
    *xe = e;
    *xd = d;
    *xn = T;
    fclose(file);

    if (result == 0)
        return 1;
    else
        return 0;
}

int CheckUidApi(char id[])
{
    char userpath[FILEPATH];
    sprintf(userpath, "%s\\data\\user.txt", AppPath);
    FILE* file = fopen(userpath, "a+");

    char buffer[FILEBUFFER] = { 0 };
    while (fgets(buffer, FILEBUFFER, file) != NULL)
    {
        char uid[9] = { 0 };

        if (buffer[0] == '\n') continue;

        for (int i = 0; buffer[i] != ';'; i++)
            uid[i] = buffer[i];

        if (!strcmp(uid, id)) return 1;
    }
    return 0;
}



