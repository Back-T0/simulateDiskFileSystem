
#pragma once
#include <iostream>
#include <vector>
#include "disk.h"
#include "myfile.h"

class DiskUtil
{
public:
    DiskUtil(DISK* disk, Myfile* root) :disk1(disk), root1(root)
    {
        current = root;
    }
    ~DiskUtil();

    Myfile* loadDisk(); //读取磁盘，生成文件实体，返回生成的文件实体根目录
    void creatFile(char a[8]); //创建文件实体
    void traverseDirEntries(int dex); //遍历目录项
    void loadCatalog(Myfile* file); //加载目录

private:
    DISK* disk1; //记录磁盘
    Myfile* root1; //记录根目录
    Myfile* current; //记录当前生成的文件
};

