
#pragma once
#include "disk.h"
#include "myfile.h"
#include "oftile.h"
/*
 *��������⣺
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
    /// ������
    /// </summary>
    void run();
    /// <summary>
    /// ��ʾFAT
    /// </summary>
    void showFAT();
    /// <summary>
    /// �ļ��б�
    /// </summary>
    void fileList();
    /// <summary>
    /// ��ʾ�ļ��б�
    /// </summary>
    /// <param name="dex">���ļ��б��е� ��dex�� ���</param>
    void showFileList(int dex);

    /// <summary>
    /// �ļ��б����
    /// </summary>
    void filelistOperate();
    /// <summary>
    /// ��ʾ�ļ������б�
    /// </summary>
    /// <param name="dex">���ļ������б�� ��dex�� ���</param>
    void showFileListOperate(int dex);

    /// <summary>
    /// �ļ����б�
    /// </summary>
    void OFTL();
    /// <summary>
    /// ��ʾ�ļ����б�
    /// </summary>
    /// <param name="dex">���ļ����б�� ��row�� ���</param>
    void showOFTL(int dex);
    /// <summary>
    /// ��ʾ�ļ����б����
    /// </summary>
    /// <param name="dex">���ļ����б������ ��dex�� ���</param>
    void showOFTLoperate(int dex);

    /// <summary>
    /// ����Ŀ¼ѡ��
    /// </summary>
    void treeLst();
    /// <summary>
    /// ��ʾ����Ŀ¼
    /// </summary>
    /// <param name="dex">������Ŀ¼�� ��dex�� ���</param>
    void showTreeLst(int dex);

private:
    DISK* disk; //����
    Myfile* root; //��Ŀ¼
    Myfile* currentCatalog; //��ǰ�ļ���
    Myfile* currentFile; //��ǰ�ļ�
    int currentOFTL = 1; //��ǰ���ļ��б�ָ��
    vector<OFTILE*> openfile; //�ļ����б�
    vector<Myfile*> allfile; //�洢���е��ļ�
    vector<string> fileOperate =
    { "���ļ�", "�½��ļ�", "�½��ļ���", "ɾ���ļ�", "�����ļ�", "��������", "��ʾ�Ѵ��ļ��б�", "��ǰĿ¼��Ϣ", "������һ��", "����" };

    /// <summary>
    /// ���ļ�
    /// </summary>
    /// <param name="row">�ļ�λ�ڵڼ���</param>
    void openFile(int& row);
    /// <summary>
    /// �½��ļ�
    /// </summary>
    void newFile();
    /// <summary>
    /// �½��ļ���
    /// </summary>
    void newCatalog();
    /// <summary>
    /// ɾ���ļ�
    /// </summary>
    void deleteFile();
    /// <summary>
    /// �����ļ�
    /// </summary>
    void searchFile();
    /// <summary>
    /// ��������
    /// </summary>
    void changeFile();
    /// <summary>
    /// ������һ��
    /// </summary>
    /// <param name="row"></param>
    void backLevel(int& row);
    /// <summary>
    /// ��ȡ���е��ļ���allfile��
    /// </summary>
    /// <param name="root">�ļ�ϵͳ�ĸ�Ŀ¼</param>
    void getAllFile(Myfile* root);
    /// <summary>
    /// ����ļ������б���
    /// </summary>
    /// <param name="">Ҫ��ӵ��ļ�</param>
    void addOFTILE(Myfile*);
    /// <summary>
    /// ���ļ��Ӵ��б������Ƴ�
    /// </summary>
    /// <param name="">Ҫ�Ƴ����ļ�</param>
    void deleteOFTILE(Myfile*);
};

