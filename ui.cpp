
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
    cout << "当前目录：";
    cout << currentCatalog->getPath() << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "\t文件名\t文件路径\t文件夹\t起始盘块号\t占用盘块列表" << endl;
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
            cout << "文件";
        }
        else
        {
            cout << "文件夹";
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
    cout << "文件打开列表：" << endl;
    cout <<
        "------------------------------------------------------------------------------------------------------------------------"
        << endl;
    cout << "    "
        << setw(16) << "文件绝对路径"
        << setw(16) << "文件属性"
        << setw(25) << "文件起始盘块号"
        << setw(16) << "文件长度"
        << setw(16) << "操作类型"
        << setw(8) << "dnum"
        << setw(12) << "bnum"
        << setw(12) << "dnum"
        << setw(12) << "bnum" << endl;
    int dex = 1; //选中指针
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
    cout << "1\t写文件" << endl
        << "2\t读文件" << endl
        << "3\t关闭文件" << endl
        << endl;

    string in;
    input = getch();
    int temp = 1;
    switch (input)
    {
    case '1':
        cout << "文件" << openfile.at(dex - 1)->name << "的内容为:\t"
            << openfile.at(dex - 1)->file->getText() << endl
            << "请输入你要写入的的内容,不写入输入N:" << endl;
        cin >> in;
        if (in.compare("N") != 0)
        {
            currentFile->setText(in);

            //改写指针块
            auto op = openfile.at(dex - 1);
            int bn = op->file->getBlockList().size() - 1;
            op->write->dnum = op->file->getBlockList().at(bn);
            op->write->bnum = in.length() % 64 - 1;

            system("cls");
            cout << "修改成功!" << endl;
            _sleep(800);
        }
        break;
    case '2':
        cout << "文件" << openfile.at(dex - 1)->name << "的内容为"
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
        cout << "文件关闭成功!" << endl;
        break;
    default:
        break;
    }
}

void UI::showFileListOperate(int dex)
{
    system("cls");
    cout << "当前路径: " << currentCatalog->getPath();
    if (currentFile == nullptr)
    {
        cout << ", 未指定文件" << endl;
    }
    else
    {
        cout << ", 当前文件: " << currentFile->getName() << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;
    //显示所有文件操作
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
    char input; //输入字符
    int row = 1; //当前指针
    bool out = false; //退出标志为否
    while (out != true)
    {
        //显示文件操作列表
        showFileListOperate(row);
        //获取输入字符
        input = getch();
        switch (input)
        {
            //指针上移
        case 'w':
            row--;
            if (row < 1)
            {
                row = fileOperate.size();
            }
            break;
            //指针下移
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

            //选定
        case '\r':
            //根据指针执行相应的操作
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
        cout << "未选定文件！" << endl;
        _sleep(800);
        return;
    }
    if (currentFile->isCatalog() == true)
    {
        currentCatalog = currentFile;
        currentFile = currentCatalog->getChildren(0);
        row = 0;
        system("cls");
        cout << "文件夹" << currentCatalog->getName() << "已打开" << endl;
        _sleep(800);
    }
    else
    {
        cout << "文件" << currentFile->getName() << "的内容为" << currentFile->getText() << endl
            << "请输入你要修改的的内容,以#号结束输入,不修改输入N:" << endl;
        char inp[1024];
        char c;
        while ((c = getchar()) != '\n'); //清空输入缓存
        scanf("%[^#]", inp);
        string in = inp;
        if (in.compare("N") != 0)
        {
            currentFile->setText(in);
            system("cls");
            cout << "修改成功!" << endl;
            _sleep(800);
            addOFTILE(currentFile);
        }
        else
        {
            addOFTILE(currentFile);
            //文件打开方式改为只读
            OFTILE* af = openfile.at(openfile.size() - 1);
            af->flag = 0;
            af->write->bnum = af->read->bnum;
            af->write->dnum = af->read->dnum;
        }
    }
}

void UI::newFile()
{
    cout << "请命名新建文件(5字符以内)：" << endl;
    char c;
    while ((c = getchar()) != '\n'); //清空输入缓存
    string in;
    cin >> in;
    if (in.length() > 5)
    {
        cout << "文件名超过5个字符！" << endl;
        _sleep(800);
        return;
    }
    if (currentCatalog->searchFile(in, false) != nullptr)
    {
        system("cls");
        cout << "文件夹" << currentCatalog->getName() << "中已有同名文件" << in << "!" << endl;
        _sleep(800);
    }
    else
    {
        auto file = new Myfile(disk, in, false, currentCatalog);
        currentCatalog->addFile(file);
        currentFile = file;
        system("cls");
        cout << "文件" << in << "新建成功!" << endl;
        _sleep(800);
    }
}

void UI::newCatalog()
{
    cout << "请命名新建文件夹(5字符以内)：" << endl;
    char c;
    while ((c = getchar()) != '\n'); //清空输入缓存
    string in;
    cin >> in;
    if (in.length() > 5)
    {
        cout << "文件名超过5个字符！" << endl;
        _sleep(800);
        return;
    }
    if (currentCatalog->searchFile(in, true) != nullptr)
    {
        system("cls");
        cout << "文件夹" << currentCatalog->getName() << "中已有同名文件夹" << in << "!" << endl;
        _sleep(800);
    }
    else
    {
        auto file = new Myfile(disk, in, true, currentCatalog);
        currentCatalog->addFile(file);
        system("cls");
        cout << "文件夹" << in << "新建成功!" << endl;
        currentFile = file;
        _sleep(800);
    }
}

void UI::deleteFile()
{
    if (currentFile == nullptr)
    {
        system("cls");
        cout << "未选定文件！" << endl;
        _sleep(800);
        return;
    }
    cout << "确认删除文件" << currentFile->getName() << "? (是请按任意键,否请按n)" << endl;
    char in = getch();
    if (in == 'n')
    {
    }
    else
    {
        //如果在文件已打开列表中，则删除
        deleteOFTILE(currentFile);
        //从父目录中移除
        currentCatalog->deleteChildren(currentFile);
        //清空磁盘
        currentFile->delectFile();
        //int x = currentFile->getBlockList().at(0);
        //disk->change_FAT(x, 0);
        //currentFile->getBlockList().clear();

        system("cls");
        cout << "文件" << currentFile->getName() << "已经删除";
        currentFile = nullptr;
        _sleep(800);
    }
}

void UI::searchFile()
{
    cout << "请输入文件名: ";
    char c;
    while ((c = getchar()) != '\n'); //清空输入缓存
    string in;
    cin >> in;
    Myfile* file;
    cout << "是否进行全局搜索? (搜索当前目录请按n,全局搜索请按任意键)";
    char a = getch();
    if (a == 'n')
    {
        //查找文件
        file = currentFile->searchFile(in, false);
        if (file != nullptr)
        {
            system("cls");
            cout << "存在此文件,路径为: "
                << file->getPath() << endl;
        }
        //查找文件夹
        else
        {
            file = currentFile->searchFile(in, true);
            if (file != nullptr)
            {
                system("cls");
                cout << "存在此文件夹,路径为: "
                    << file->getPath() << endl;
            }
        }
    }
    else
    {
        //查找文件
        file = root->search(in, false);
        if (file != nullptr)
        {
            system("cls");
            cout << "存在此文件,路径为: "
                << file->getPath() << endl;
        }
        //查找文件夹
        else
        {
            file = root->search(in, true);
            if (file != nullptr)
            {
                system("cls");
                cout << "存在此文件夹,路径为: "
                    << file->getPath() << endl;
            }
        }
    }
    system("pause");
}

void UI::changeFile()
{
    //读取属性
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
    //显示修改操作
    char input;
    bool out = false;
    while (out != true)
    {
        system("cls");
        itoa((int)attributes, str, 2);
        cout << "该文件属性为：0b" << str << endl;
        cout << "1. 只读文件：" << isReadOnly << endl;
        cout << "2. 系统文件：" << isSysFiles << endl;
        cout << "3. 普通文件：" << isNormalFile << endl;
        cout << "4. 目录属性：" << isContents << endl;
        cout << "请输入要修改的属性，输入#号键返回上级：";
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
    //写回属性

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
        cout << "已返回上一层,路径为" << currentCatalog->getPath() << endl;
    }
    else
    {
        system("cls");
        cout << "已经是根目录!" << endl;
    }
    _sleep(800);
}

void UI::addOFTILE(Myfile* file)
{
    //如果已经打开了文件，关闭再打开
    deleteOFTILE(file);
    //根据文件设置文件打开列表属性
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
    //清空磁盘
    fclose(disk->fp);
    disk->fp = fopen("Disk", "w+");
    disk->initDisk();
    getAllFile(root);
    int p = 1;
    system("cls");
    showFAT();
    cout << "当前路径：";
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
            cout << "(文件)";
        }
        else
        {
            cout << "(文件夹)";
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
