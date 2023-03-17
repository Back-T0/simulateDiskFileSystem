
#pragma once
#include "myfile.h"
#pragma warning(disable:4996)
class pointer
{
public:
    int dnum; //�����̿��
    int bnum; //�����̿��ڵڼ����ֽ�
    pointer()
    {
        dnum = 0;
        bnum = 0;
    }
    pointer(int dnum, int bnum)
    {
        this->dnum = dnum;
        this->bnum = bnum;
    }
};

class OFTILE
{
public:
    char name[128]; //�ļ�����·����
    char attribute; //�ļ������ԣ��� 1 ���ֽڱ�ʾ�����Դ��� char ����
    int number; //�ļ���ʼ�̿��
    int length; //�ļ����ȣ��ļ�ռ�õ��ֽ���
    int flag; //�������ͣ��á�0����ʾ�Զ�������ʽ���ļ����á�1����ʾ��д������ʽ���ļ�
    pointer* read; //���ļ���λ�ã��ļ���ʱ dnum Ϊ�ļ���ʼ�̿�ţ�bnum Ϊ��0��
    pointer* write; //д�ļ���λ�ã��ļ��ս���ʱ dnum Ϊ�ļ���ʼ�̿�ţ�bnum Ϊ��0 �����ļ�ʱ dnum �� bnum Ϊ�ļ���ĩβλ��
    Myfile* file;    //�ļ�

    OFTILE(Myfile* file, char name[128], char attribute, int number, int length, int flag, pointer* read = nullptr, pointer* write = nullptr)
        :file(file), attribute(attribute), number(number), length(length)
    {
        strncpy(this->name, name, 128);
        this->flag = flag;
        this->read = read;
        this->write = write;
    }
    ~OFTILE()
    {
        delete read;
        delete write;
        delete file;
    }
};


