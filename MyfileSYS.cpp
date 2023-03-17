
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include "ui.h"
#include "disk.h"
#include "myfile.h"
#include "loadFromDisk.h"

int main()
{
    DISK* disk;
    Myfile* root;
    //����Ƿ������ļ�
    char filePath[128] = "Disk";
    FILE* fp = fopen(filePath, "r");
    bool isExist = false;
    if (fp != nullptr) {
        isExist = true;
        fclose(fp);
    }
    //delete fp;
    if (isExist) {
        cout << "��⵽�����ļ����Ƿ��ȡ��y/n" << endl;
        char inp;
        inp = getch();
        if (inp == 'y')
        {
            disk = new DISK(filePath);//�½�����
            root = new Myfile(disk, "root");//�½���Ŀ¼
            root->setStartDex(2);//���ø�Ŀ¼��ʼ�̿�
            DiskUtil* util = new DiskUtil(disk, root);
            root = util->loadDisk();//���ش���
            auto ui = new UI(disk, root); //�½�����
            ui->run(); //���н���
            return 0;
        }
    }
    disk = new DISK(); //�½�����
    root = new Myfile(disk, "root", true); //�½���Ŀ¼
    auto ui = new UI(disk, root); //�½�����
    ui->run(); //���н���
    return 0;
}

