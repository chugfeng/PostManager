#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "loginService.h"
#include "userService.h"



RsaPriKey pkey;

/**
 * ��¼��API.
 * \param id ��¼��id
 * \param pass ��¼������
 * \param re ��¼�ɹ����ȡ�Ĺ�Կ��e
 * \param rn ��¼�ɹ����ȡ�Ĺ�Կ��n
 * \return 1��¼�ɹ� 0��¼ʧ��
 */
int LoginApi(char id[],char pass[],int *re,int *rn)
{
    int pd;
    int pn;
    char path[MAXPATH] = { 0 };
    char buffer[MAXFILEBUFFER] = { 0 };
    int isTrue = 0;

    strcpy(path,AppPath);
    strcat(path,"\\data\\user.txt");

    FILE *file = fopen(path, "r");

    if (file == NULL)
	{
		printf("%s �ļ������ڣ�����\n", path);
		return isTrue;
	}

	//���ж�ȡ�ļ�
	while (fgets(buffer, MAXFILEBUFFER, file) != NULL)
	{
	    int bufferIndex = 0;
	    //�Ȼ�ȡÿ�е�id
	    char uid[8] = {0};
	    for(int i = 0; buffer[i] != ';';i++)
        {
            uid[i] = buffer[i];
            bufferIndex ++;

            if (buffer[i] == '\n') break;
        }
        bufferIndex ++;

        //�ж��Ƿ�ʹ����������,//Ϊ���ʱ��˵�������,����һ��
        if(strcmp(uid,id))
           continue;

        //��ȡ���˺ź�˵�� ������˻��˻�ע��������Դ�˽Կ�����ȡ������˻���˽Կ
        //��ȡ���ļ�
        int isGetPriKey = 0;
        char priKeyPath [MAXPATH] = {0};
        char pvbuffe[MAXFILEBUFFER]={0};

        sprintf(priKeyPath,"%s\\data\\privatekey.txt",AppPath);
        FILE *pvFile = fopen(priKeyPath,"r");

        if (pvFile == NULL)
        {
            printf("%s �ļ������ڣ�����", priKeyPath);
            return 0;
        }

        while (fgets(pvbuffe, MAXFILEBUFFER, pvFile) != NULL)
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
        fclose(pvFile);     //�ͷ�ָ��
        if(isGetPriKey == 0)
        {
            printf("δ��ȡ�����˽Կ������ϵϵͳ������Ա�ύbug\n");
            isTrue = 0;
            return isTrue;
        }

        //�˺�����˻�ȡ����
        char pwd[16] = {0};;
        for(int i = bufferIndex ,j = 0; buffer[i] != ';';i++,j++)
            pwd[j] = buffer[i];

        //����ȡ����������ܣ�������������ǽ���������ݽ��м��ܣ�����������

        //�ж������Ƿ����
        if(! strcmp(pwd,pass))
        {
            isTrue = 1;
            break;
        }else
            break;
	}
	return isTrue;
}



