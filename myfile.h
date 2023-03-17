
#ifndef MYFILE_H
#define MYFILE_H

#include <iostream>
#include <vector>
#include "disk.h"
using namespace std;

class Myfile
{
public:
    Myfile(DISK* disk, string fileName, bool isCatalog, Myfile* catalog = nullptr);
    Myfile(DISK* disk, string fileName);
    ~Myfile();

    //数据封装

    bool isCatalog();
    void setName(string name);
    void setText(string text, bool getB = false);
    void setPropertie(char propertie);
    void setStartDex(int dex);
    void addBlocklist(int dex);
    void addChildren(Myfile* children);
    void setParent(Myfile* parentcatalog);
    void setPath(string path);
    void setIsCatalog(bool iscatalog);
    void setLevel(int lev);
    string getName();
    Myfile* getChildren(int index);
    Myfile* getParent();
    int getBlockCount();
    int getStartDex();
    vector<int> getBlockList();
    char getPropertie();
    string getText();
    int getSize();
    string getPath();
    int getLevel();
    DISK* getDisk()
    {
        return disk;
    };
    vector<Myfile*> getChildrens()
    {
        return fileList;
    }
    int getChildrenCount()
    {
        return fileList.size();
    }

    //文件操作

    Myfile* searchFile(string fileName, bool iscatalog);
    Myfile* search(string fileName, bool iscatalog);
    bool addFile(Myfile* file);
    void delectFile();
    void deleteChildren(Myfile* children);
    void changePath(Myfile* parent);

    //磁盘操作

    void writeFileToDisk();

private:
    DISK* disk = nullptr; //所属磁盘
    bool iscatalog = false; //是否文件夹
    string name; //文件名
    string path; //文件路径
    Myfile* parentcatalog = nullptr; //父目录
    vector<Myfile*> fileList; //如果是文件夹则有文件列表
    vector<int> blockList; //盘块列表
    char propertie; //文件属性
    string text; //文件内容
    int level = 0;
};

#endif // MYFILE_H

