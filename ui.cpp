
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include "ui.h"
using namespace std;
#pragma warning(disable:4996)

void UI::run()
{
    treeLst();
}

void UI::fileList()
{
    char input;
    int row = 1;
    while (true)
    {
        showFAT();
        showFileList(row);
        input = getch();
        switch (input)
        {
        case 'w':
            row--;
            if (row < 1)
            {
                row = currentCatalog->getChildrens().size();
            }
            if (row != 0)
            {
                currentFile = currentCatalog->getChildrens().at(row - 1);
            }
            break;
        case 's':
            row++;
            if (row > currentCatalog->getChildrens().size())
            {
                row = 1;
            }
            if (row != 0)
            {
                currentFile = currentCatalog->getChildrens().at(row - 1);
            }
            break;
        case 'a':
            backLevel(row);
            row = 0;
            break;
        case 'd':
            openFile(row);
            row = 0;
            break;
        case '\r':
            filelistOperate();
            break;
        default:
            break;
        }
        system("cls");
    }
}

void UI::showFAT()
{
    cout << "------------------------------FAT------------------------------" << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            printf("%3d ", disk->get_FAT(j + i * 16));
        }
        cout << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
}

void UI::showFileList(int row)
{
    cout << "��ǰĿ¼��";
    cout << currentCatalog->getPath() << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "\t�ļ���\t�ļ�·��\t�ļ���\t��ʼ�̿��\tռ���̿��б�" << endl;
    int dex = 1;
    for (auto file : currentCatalog->getChildrens())
    {
        if (dex == row)
        {
            cout << "-->\t";
        }
        else
        {
            cout << "\t";
        }
        dex++;
        cout << file->getName() << "\t"
            << file->getPath() << "\t";
        if (file->isCatalog() == false)
        {
            cout << "�ļ�";
        }
        else
        {
            cout << "�ļ���";
            cout << file->getChildrenCount();
        }
        cout << "\t"
            << file->getStartDex() << "\t\t";
        for (auto i : file->getBlockList())
        {
            cout << i << ' ';
        }
        cout << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
}

void UI::showOFTL(int row)
{
    system("cls");
    cout << "�ļ����б�" << endl;
    cout <<
        "------------------------------------------------------------------------------------------------------------------------"
        << endl;
    cout << "    "
        << setw(16) << "�ļ�����·��"
        << setw(16) << "�ļ�����"
        << setw(25) << "�ļ���ʼ�̿��"
        << setw(16) << "�ļ�����"
        << setw(16) << "��������"
        << setw(8) << "dnum"
        << setw(12) << "bnum"
        << setw(12) << "dnum"
        << setw(12) << "bnum" << endl;
    int dex = 1; //ѡ��ָ��
    for (auto op : openfile)
    {
        if (dex == row)
        {
            cout << "--> ";
        }
        else
        {
            cout << "    ";
        }
        dex++;
        cout << setw(12) << op->name << setw(12)
            << op->attribute << setw(12)
            << op->number << setw(12)
            << op->length << setw(12)
            << op->flag << setw(12)
            << op->read->dnum << setw(12)
            << op->read->bnum << setw(12)
            << op->write->dnum << setw(12)
            << op->write->bnum
            << endl;
    }
    cout <<
        "------------------------------------------------------------------------------------------------------------------------"
        << endl;
}

void UI::OFTL()
{
    char input;
    bool out = false;
    while (out != true)
    {
        showOFTL(currentOFTL);
        input = getch();
        switch (input)
        {
        case 'w':
        {
            currentOFTL--;
            if (currentOFTL < 1)currentOFTL = openfile.size();
            break;
        }
        case 's':
        {
            currentOFTL++;
            if (currentOFTL > openfile.size())currentOFTL = 1;
            break;
        }
        case 'a':
            out = true;
            break;
        case '\r':
        {
            showOFTLoperate(currentOFTL);
        }
        default:
            break;
        }
        _sleep(800);
    }
}

void UI::showOFTLoperate(int dex)
{
    char input;
    system("cls");
    cout << "1\tд�ļ�" << endl
        << "2\t���ļ�" << endl
        << "3\t�ر��ļ�" << endl
        << endl;

    string in;
    input = getch();
    int temp = 1;
    switch (input)
    {
    case '1':
        cout << "�ļ�" << openfile.at(dex - 1)->name << "������Ϊ:\t"
            << openfile.at(dex - 1)->file->getText() << endl
            << "��������Ҫд��ĵ�����,��д������N:" << endl;
        cin >> in;
        if (in.compare("N") != 0)
        {
            currentFile->setText(in);

            //��дָ���
            auto op = openfile.at(dex - 1);
            int bn = op->file->getBlockList().size() - 1;
            op->write->dnum = op->file->getBlockList().at(bn);
            op->write->bnum = in.length() % 64 - 1;

            system("cls");
            cout << "�޸ĳɹ�!" << endl;
            _sleep(800);
        }
        break;
    case '2':
        cout << "�ļ�" << openfile.at(dex - 1)->name << "������Ϊ"
            << openfile.at(dex - 1)->file->getText() << endl;
        break;
    case '3':
        for (auto it = openfile.begin(); it != openfile.end(); ++it)
        {
            if (temp == dex)
            {
                openfile.erase(it);
                currentOFTL = 0;
                break;
            }
            temp++;
        }
        cout << "�ļ��رճɹ�!" << endl;
        break;
    default:
        break;
    }
}

void UI::showFileListOperate(int dex)
{
    system("cls");
    cout << "��ǰ·��: " << currentCatalog->getPath();
    if (currentFile == nullptr)
    {
        cout << ", δָ���ļ�" << endl;
    }
    else
    {
        cout << ", ��ǰ�ļ�: " << currentFile->getName() << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;
    //��ʾ�����ļ�����
    for (unsigned int j = 1; j <= fileOperate.size(); j++)
    {
        if (j == dex)
        {
            cout << "-->\t";
        }
        else
        {
            cout << '\t';
        }
        cout << fileOperate.at(j - 1) << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;
}

void UI::filelistOperate()
{
    char input; //�����ַ�
    int row = 1; //��ǰָ��
    bool out = false; //�˳���־Ϊ��
    while (out != true)
    {
        //��ʾ�ļ������б�
        showFileListOperate(row);
        //��ȡ�����ַ�
        input = getch();
        switch (input)
        {
            //ָ������
        case 'w':
            row--;
            if (row < 1)
            {
                row = fileOperate.size();
            }
            break;
            //ָ������
        case 's':
            row++;
            if (row > fileOperate.size())
            {
                row = 1;
            }
            break;
        case 'a':
            out = true;
            break;
        case 'd':

            //ѡ��
        case '\r':
            //����ָ��ִ����Ӧ�Ĳ���
            switch (row)
            {
            case 1:
                openFile(row);
                out = false;
                break;
            case 2:
                newFile();
                break;
            case 3:
                newCatalog();
                break;
            case 4:
                deleteFile();
                break;
            case 5:
                searchFile();
                break;
            case 6:
                changeFile();
                break;
            case 7:
                OFTL();
                break;
            case 8:
                showFileList(0);
                system("pause");
                out = true;
                break;
            case 9:
                backLevel(row);
                out = true;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void UI::openFile(int& row)
{
    if (currentFile == nullptr)
    {
        system("cls");
        cout << "δѡ���ļ���" << endl;
        _sleep(800);
        return;
    }
    if (currentFile->isCatalog() == true)
    {
        currentCatalog = currentFile;
        currentFile = currentCatalog->getChildren(0);
        row = 0;
        system("cls");
        cout << "�ļ���" << currentCatalog->getName() << "�Ѵ�" << endl;
        _sleep(800);
    }
    else
    {
        cout << "�ļ�" << currentFile->getName() << "������Ϊ" << currentFile->getText() << endl
            << "��������Ҫ�޸ĵĵ�����,��#�Ž�������,���޸�����N:" << endl;
        char inp[1024];
        char c;
        while ((c = getchar()) != '\n'); //������뻺��
        scanf("%[^#]", inp);
        string in = inp;
        if (in.compare("N") != 0)
        {
            currentFile->setText(in);
            system("cls");
            cout << "�޸ĳɹ�!" << endl;
            _sleep(800);
            addOFTILE(currentFile);
        }
        else
        {
            addOFTILE(currentFile);
            //�ļ��򿪷�ʽ��Ϊֻ��
            OFTILE* af = openfile.at(openfile.size() - 1);
            af->flag = 0;
            af->write->bnum = af->read->bnum;
            af->write->dnum = af->read->dnum;
        }
    }
}

void UI::newFile()
{
    cout << "�������½��ļ�(5�ַ�����)��" << endl;
    char c;
    while ((c = getchar()) != '\n'); //������뻺��
    string in;
    cin >> in;
    if (in.length() > 5)
    {
        cout << "�ļ�������5���ַ���" << endl;
        _sleep(800);
        return;
    }
    if (currentCatalog->searchFile(in, false) != nullptr)
    {
        system("cls");
        cout << "�ļ���" << currentCatalog->getName() << "������ͬ���ļ�" << in << "!" << endl;
        _sleep(800);
    }
    else
    {
        auto file = new Myfile(disk, in, false, currentCatalog);
        currentCatalog->addFile(file);
        currentFile = file;
        system("cls");
        cout << "�ļ�" << in << "�½��ɹ�!" << endl;
        _sleep(800);
    }
}

void UI::newCatalog()
{
    cout << "�������½��ļ���(5�ַ�����)��" << endl;
    char c;
    while ((c = getchar()) != '\n'); //������뻺��
    string in;
    cin >> in;
    if (in.length() > 5)
    {
        cout << "�ļ�������5���ַ���" << endl;
        _sleep(800);
        return;
    }
    if (currentCatalog->searchFile(in, true) != nullptr)
    {
        system("cls");
        cout << "�ļ���" << currentCatalog->getName() << "������ͬ���ļ���" << in << "!" << endl;
        _sleep(800);
    }
    else
    {
        auto file = new Myfile(disk, in, true, currentCatalog);
        currentCatalog->addFile(file);
        system("cls");
        cout << "�ļ���" << in << "�½��ɹ�!" << endl;
        currentFile = file;
        _sleep(800);
    }
}

void UI::deleteFile()
{
    if (currentFile == nullptr)
    {
        system("cls");
        cout << "δѡ���ļ���" << endl;
        _sleep(800);
        return;
    }
    cout << "ȷ��ɾ���ļ�" << currentFile->getName() << "? (���밴�����,���밴n)" << endl;
    char in = getch();
    if (in == 'n')
    {
    }
    else
    {
        //������ļ��Ѵ��б��У���ɾ��
        deleteOFTILE(currentFile);
        //�Ӹ�Ŀ¼���Ƴ�
        currentCatalog->deleteChildren(currentFile);
        //��մ���
        currentFile->delectFile();
        //int x = currentFile->getBlockList().at(0);
        //disk->change_FAT(x, 0);
        //currentFile->getBlockList().clear();

        system("cls");
        cout << "�ļ�" << currentFile->getName() << "�Ѿ�ɾ��";
        currentFile = nullptr;
        _sleep(800);
    }
}

void UI::searchFile()
{
    cout << "�������ļ���: ";
    char c;
    while ((c = getchar()) != '\n'); //������뻺��
    string in;
    cin >> in;
    Myfile* file;
    cout << "�Ƿ����ȫ������? (������ǰĿ¼�밴n,ȫ�������밴�����)";
    char a = getch();
    if (a == 'n')
    {
        //�����ļ�
        file = currentFile->searchFile(in, false);
        if (file != nullptr)
        {
            system("cls");
            cout << "���ڴ��ļ�,·��Ϊ: "
                << file->getPath() << endl;
        }
        //�����ļ���
        else
        {
            file = currentFile->searchFile(in, true);
            if (file != nullptr)
            {
                system("cls");
                cout << "���ڴ��ļ���,·��Ϊ: "
                    << file->getPath() << endl;
            }
        }
    }
    else
    {
        //�����ļ�
        file = root->search(in, false);
        if (file != nullptr)
        {
            system("cls");
            cout << "���ڴ��ļ�,·��Ϊ: "
                << file->getPath() << endl;
        }
        //�����ļ���
        else
        {
            file = root->search(in, true);
            if (file != nullptr)
            {
                system("cls");
                cout << "���ڴ��ļ���,·��Ϊ: "
                    << file->getPath() << endl;
            }
        }
    }
    system("pause");
}

void UI::changeFile()
{
    //��ȡ����
    char attributes = currentFile->getPropertie();
    char str[25];
    itoa((int)attributes, str, 2);
    int len = sizeof(str);
    bool isReadOnly = false;
    bool isSysFiles = false;
    bool isNormalFile = false;
    bool isContents = false;
    if (--len >= 0 && str[len] == '1') {
        isReadOnly = true;
    }
    if (--len >= 0 && str[len] == '1') {
        isSysFiles = true;
    }
    if (--len >= 0 && str[len] == '1') {
        isNormalFile = true;
    }
    if (--len >= 0 && str[len] == '1') {
        isContents = true;
    }
    //��ʾ�޸Ĳ���
    char input;
    bool out = false;
    while (out != true)
    {
        system("cls");
        itoa((int)attributes, str, 2);
        cout << "���ļ�����Ϊ��0b" << str << endl;
        cout << "1. ֻ���ļ���" << isReadOnly << endl;
        cout << "2. ϵͳ�ļ���" << isSysFiles << endl;
        cout << "3. ��ͨ�ļ���" << isNormalFile << endl;
        cout << "4. Ŀ¼���ԣ�" << isContents << endl;
        cout << "������Ҫ�޸ĵ����ԣ�����#�ż������ϼ���";
        input = getch();
        switch (input)
        {
        case '1':
            isReadOnly = !isReadOnly;
            break;
        case '2':
            isSysFiles = !isSysFiles;
            break;
        case '3':
            isNormalFile = !isNormalFile;
            break;
        case '4':
            isContents = !isContents;
            break;
        case 'a':
        case '#':
            out = true;
            break;
        default:
            break;
        }
        attributes = 0b000000000;
        if (isReadOnly)
        {
            attributes += 0b1;
        }
        if (isSysFiles)
        {
            attributes += 0b10;
        }
        if (isNormalFile)
        {
            attributes += 0b100;
        }
        if (isContents)
        {
            attributes += 0b1000;
        }
    }
    //д������

    currentFile->setPropertie(attributes);
}

void UI::backLevel(int& row)
{
    if (currentCatalog->getParent() != nullptr)
    {
        currentFile = currentCatalog;
        currentCatalog = currentCatalog->getParent();
        if (currentCatalog->getChildrenCount() == 0)
        {
            row = 0;
        }
        else
        {
            row = 1;
            for (auto it : currentCatalog->getChildrens())
            {
                if (it == currentFile)
                {
                    break;
                }
                row++;
            }
        }
        system("cls");
        cout << "�ѷ�����һ��,·��Ϊ" << currentCatalog->getPath() << endl;
    }
    else
    {
        system("cls");
        cout << "�Ѿ��Ǹ�Ŀ¼!" << endl;
    }
    _sleep(800);
}

void UI::addOFTILE(Myfile* file)
{
    //����Ѿ������ļ����ر��ٴ�
    deleteOFTILE(file);
    //�����ļ������ļ����б�����
    int dnum = file->getStartDex();
    int bnum = 0;
    auto read = new pointer(dnum, bnum);
    dnum = file->getBlockList().at(file->getBlockList().size() - 1);
    int number = dnum;
    bnum = file->getText().length() % 64 - 1;
    auto write = new pointer(dnum, bnum);
    string str = file->getPath();
    char name[128] = { 0 };
    strncpy(name, str.c_str(), str.length() + 1);
    char attribute = file->getPropertie();
    int length = file->getText().length();
    int flag = 1;
    auto afile = new OFTILE(file, name, attribute, number, length, flag, read, write);
    openfile.push_back(afile);
}

void UI::deleteOFTILE(Myfile* file)
{
    for (auto it = openfile.begin(); it != openfile.end(); ++it)
    {
        if ((*it)->number == file->getStartDex())
        {
            openfile.erase(it);
            break;
        }
    }
}

void UI::showTreeLst(int row)
{
    allfile.clear();
    //��մ���
    fclose(disk->fp);
    disk->fp = fopen("Disk", "w+");
    disk->initDisk();
    getAllFile(root);
    int p = 1;
    system("cls");
    showFAT();
    cout << "��ǰ·����";
    cout << currentCatalog->getPath() << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    for (auto file : allfile)
    {
        if (p == row)
        {
            cout << "-->";
        }
        else
        {
            cout << "   ";
        }
        for (int i = 0; i < file->getLevel(); i++)
        {
            cout << "\t";
        }
        cout << file->getName();
        if (file->isCatalog() == false)
        {
            cout << "(�ļ�)";
        }
        else
        {
            cout << "(�ļ���)";
            cout << file->getChildrenCount();
        }
        cout << "\t\t";
        for (auto i : file->getBlockList())
        {
            cout << i << ' ';
        }
        cout << endl;
        p++;
    }
    disk->updata_FAT_to_Disk();
    fclose(disk->fp);
    cout << "---------------------------------------------------------------------------" << endl;
}

void UI::getAllFile(Myfile* file)
{
    if (file == nullptr)
    {
        return;
    }
    allfile.push_back(file);
    file->writeFileToDisk();
    if (file->isCatalog())
    {
        if (file->getChildrenCount() == 0)
        {
            return;
        }
        for (auto temp : file->getChildrens())
        {
            getAllFile(temp);
        }
    }
}

void UI::treeLst()
{
    currentCatalog = root;
    currentFile = root;
    char input;
    int row = 1;
    bool out = false;
    while (out != true)
    {
        system("cls");
        showTreeLst(row);
        input = getch();
        switch (input)
        {
        case 'w':
        {
            row--;
            if (row < 1)
            {
                row = allfile.size();
            }
            if (row != 0)
            {
                currentFile = allfile.at(row - 1);
                currentCatalog = currentFile->getParent();
                if (currentCatalog == nullptr)currentCatalog = root;
            }
            break;
        }
        case 's':
        {
            row++;
            if (row > allfile.size())
            {
                row = 1;
            }
            if (row != 0)
            {
                currentFile = allfile.at(row - 1);
                currentCatalog = currentFile->getParent();
                if (currentCatalog == nullptr)currentCatalog = root;
            }
            break;
        }
        case 'a':
            backLevel(row);
            break;
        case 'd':
            openFile(row);
            break;
        case '\r':
            disk->fp = fopen("Disk", "r+");
            filelistOperate();
            break;
        default:
            break;
        }
        system("cls");
        disk->updata_FAT_to_Disk();
    }
}
