
#pragma once
#include "myfile.h"
#pragma warning(disable:4996)
class pointer
{
public:
    int dnum; //磁盘盘块号
    int bnum; //磁盘盘块内第几个字节
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
    char name[128]; //文件绝对路径名
    char attribute; //文件的属性，用 1 个字节表示，所以此用 char 类型
    int number; //文件起始盘块号
    int length; //文件长度，文件占用的字节数
    int flag; //操作类型，用“0”表示以读操作方式打开文件，用“1”表示以写操作方式打开文件
    pointer* read; //读文件的位置，文件打开时 dnum 为文件起始盘块号，bnum 为“0”
    pointer* write; //写文件的位置，文件刚建立时 dnum 为文件起始盘块号，bnum 为“0 ，打开文件时 dnum 和 bnum 为文件的末尾位置
    Myfile* file;    //文件

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


