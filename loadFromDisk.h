
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

    Myfile* loadDisk(); //��ȡ���̣������ļ�ʵ�壬�������ɵ��ļ�ʵ���Ŀ¼
    void creatFile(char a[8]); //�����ļ�ʵ��
    void traverseDirEntries(int dex); //����Ŀ¼��
    void loadCatalog(Myfile* file); //����Ŀ¼

private:
    DISK* disk1; //��¼����
    Myfile* root1; //��¼��Ŀ¼
    Myfile* current; //��¼��ǰ���ɵ��ļ�
};

