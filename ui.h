
#pragma once
#include "disk.h"
#include "myfile.h"
#include "oftile.h"
/*
 *待解决问题：
 *
 *
 *
 */
class UI
{
public:
    UI(DISK* disk, Myfile* rootFile)
    {
        this->disk = disk;
        this->root = rootFile;
        this->currentCatalog = rootFile;
        this->currentFile = rootFile->getChildren(0);
    }

    ~UI()
    {
        delete disk;
        delete currentFile;
    }

    /// <summary>
    /// 主界面
    /// </summary>
    void run();
    /// <summary>
    /// 显示FAT
    /// </summary>
    void showFAT();
    /// <summary>
    /// 文件列表
    /// </summary>
    void fileList();
    /// <summary>
    /// 显示文件列表
    /// </summary>
    /// <param name="dex">将文件列表中的 第dex项 标出</param>
    void showFileList(int dex);

    /// <summary>
    /// 文件列表操作
    /// </summary>
    void filelistOperate();
    /// <summary>
    /// 显示文件操作列表
    /// </summary>
    /// <param name="dex">将文件操作列表的 第dex项 标出</param>
    void showFileListOperate(int dex);

    /// <summary>
    /// 文件打开列表
    /// </summary>
    void OFTL();
    /// <summary>
    /// 显示文件打开列表
    /// </summary>
    /// <param name="dex">将文件打开列表的 第row项 标出</param>
    void showOFTL(int dex);
    /// <summary>
    /// 显示文件打开列表操作
    /// </summary>
    /// <param name="dex">将文件打开列表操作的 第dex项 标出</param>
    void showOFTLoperate(int dex);

    /// <summary>
    /// 树形目录选择
    /// </summary>
    void treeLst();
    /// <summary>
    /// 显示树形目录
    /// </summary>
    /// <param name="dex">将树形目录的 第dex项 标出</param>
    void showTreeLst(int dex);

private:
    DISK* disk; //磁盘
    Myfile* root; //根目录
    Myfile* currentCatalog; //当前文件夹
    Myfile* currentFile; //当前文件
    int currentOFTL = 1; //当前打开文件列表指针
    vector<OFTILE*> openfile; //文件打开列表
    vector<Myfile*> allfile; //存储所有的文件
    vector<string> fileOperate =
    { "打开文件", "新建文件", "新建文件夹", "删除文件", "查找文件", "更改属性", "显示已打开文件列表", "当前目录信息", "返回上一层", "返回" };

    /// <summary>
    /// 打开文件
    /// </summary>
    /// <param name="row">文件位于第几行</param>
    void openFile(int& row);
    /// <summary>
    /// 新建文件
    /// </summary>
    void newFile();
    /// <summary>
    /// 新建文件夹
    /// </summary>
    void newCatalog();
    /// <summary>
    /// 删除文件
    /// </summary>
    void deleteFile();
    /// <summary>
    /// 查找文件
    /// </summary>
    void searchFile();
    /// <summary>
    /// 更改属性
    /// </summary>
    void changeFile();
    /// <summary>
    /// 返回上一层
    /// </summary>
    /// <param name="row"></param>
    void backLevel(int& row);
    /// <summary>
    /// 获取所有的文件到allfile中
    /// </summary>
    /// <param name="root">文件系统的根目录</param>
    void getAllFile(Myfile* root);
    /// <summary>
    /// 添加文件到打开列表中
    /// </summary>
    /// <param name="">要添加的文件</param>
    void addOFTILE(Myfile*);
    /// <summary>
    /// 把文件从打开列表项中移除
    /// </summary>
    /// <param name="">要移除的文件</param>
    void deleteOFTILE(Myfile*);
};

