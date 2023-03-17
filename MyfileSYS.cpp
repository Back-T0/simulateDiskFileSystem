
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
    //检测是否已有文件
    char filePath[128] = "Disk";
    FILE* fp = fopen(filePath, "r");
    bool isExist = false;
    if (fp != nullptr) {
        isExist = true;
        fclose(fp);
    }
    //delete fp;
    if (isExist) {
        cout << "检测到磁盘文件，是否读取？y/n" << endl;
        char inp;
        inp = getch();
        if (inp == 'y')
        {
            disk = new DISK(filePath);//新建磁盘
            root = new Myfile(disk, "root");//新建根目录
            root->setStartDex(2);//设置根目录起始盘块
            DiskUtil* util = new DiskUtil(disk, root);
            root = util->loadDisk();//加载磁盘
            auto ui = new UI(disk, root); //新建界面
            ui->run(); //运行界面
            return 0;
        }
    }
    disk = new DISK(); //新建磁盘
    root = new Myfile(disk, "root", true); //新建根目录
    auto ui = new UI(disk, root); //新建界面
    ui->run(); //运行界面
    return 0;
}

