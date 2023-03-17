
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

    //���ݷ�װ

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

    //�ļ�����

    Myfile* searchFile(string fileName, bool iscatalog);
    Myfile* search(string fileName, bool iscatalog);
    bool addFile(Myfile* file);
    void delectFile();
    void deleteChildren(Myfile* children);
    void changePath(Myfile* parent);

    //���̲���

    void writeFileToDisk();

private:
    DISK* disk = nullptr; //��������
    bool iscatalog = false; //�Ƿ��ļ���
    string name; //�ļ���
    string path; //�ļ�·��
    Myfile* parentcatalog = nullptr; //��Ŀ¼
    vector<Myfile*> fileList; //������ļ��������ļ��б�
    vector<int> blockList; //�̿��б�
    char propertie; //�ļ�����
    string text; //�ļ�����
    int level = 0;
};

#endif // MYFILE_H

