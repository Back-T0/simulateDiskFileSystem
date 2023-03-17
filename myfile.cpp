
#include "myfile.h"
#include <algorithm>
#pragma warning(disable:4996)

/// <summary>
/// 构造函数
/// </summary>
/// <param name="disk">磁盘指针</param>
/// <param name="fileName">要创建的文件名</param>
/// <param name="iscatalog">是否文件夹</param>
/// <param name="catalog">父目录文件指针</param>
Myfile::Myfile(DISK* disk, string fileName, bool iscatalog, Myfile* catalog)
{
    //分配磁盘块
    this->disk = disk;
    int startBlock = disk->get_free_Disk_block();
    blockList.push_back(startBlock);

    //定义常规属性
    this->iscatalog = iscatalog;
    this->name = fileName;
    if (catalog != nullptr)
    {
        this->parentcatalog = catalog;
        this->path.assign(catalog->getPath());
        this->level = catalog->level + 1;
    }
    else
    {
        this->level = 0;
        this->path.append(":");
    }
    this->path.append(fileName);
    this->path.append("/");
    if (iscatalog == true)
    {
        propertie = 0b00001100;
    }
    else
    {
        propertie = 0b00000100;
    }
    //cout << "Myfile::Myfile: "<< this->name << "构建成功！"<< endl;
}

/// <summary>
/// 构造函数，生成只有文件名的的文件，需要进行设置才能使用
/// </summary>
/// <param name="disk">磁盘</param>
/// <param name="fileName">文件名</param>
Myfile::Myfile(DISK* disk, string fileName) :
    disk(disk), name(fileName)
{
    //分配磁盘块
    this->disk = disk;

    //定义常规属性
    this->iscatalog = true;
    this->name = fileName;
    this->level = 0;
    this->path.append(":");
    this->path.append(fileName);
    this->path.append("/");
    propertie = 0b00001100;
}

/// <summary>
/// 析构函数
/// </summary>
Myfile::~Myfile()
{
    fileList.clear();
    cout << "Myfile::~Myfile: "
        << this->name
        << "文件已析构"
        << endl;
}

/// <summary>
/// 遍历父目录，查找文件名
/// </summary>
/// <param name="fileName">要查找的文件名</param>
/// <param name="iscatalog">是否文件夹</param>
/// <returns></returns>
Myfile* Myfile::searchFile(string fileName, bool iscatalog)
{
    //检索目录中是否已有此文件
    if (this->iscatalog == true)
    {
        for (Myfile* temp : fileList)
        {
            if (fileName.compare(temp->name) == 0 && (temp->isCatalog() == iscatalog))
            {
                return temp;
            }
        }
    }
    //如果是这是文件，从父目录中搜索
    else
    {
        for (Myfile* temp : parentcatalog->fileList)
        {
            if (fileName.compare(temp->name) == 0 && (temp->isCatalog() == iscatalog))
            {
                return temp;
            }
        }
    }
    cout << "Myfile::searchFile: "
        << "中找不到文件"
        << fileName
        << "!"
        << endl;
    return nullptr;
}

/// <summary>
/// 遍历所有文件，查找文件名
/// </summary>
/// <param name="fileName">要查找的文件名</param>
/// <param name="iscatalog">是否文件夹</param>
/// <returns></returns>
Myfile* Myfile::search(string fileName, bool iscatalog)
{
    //检索目录中是否已有此文件
    for (Myfile* temp : this->fileList)
    {
        if (fileName.compare(temp->name) == 0 && (temp->iscatalog == iscatalog))
        {
            return temp;
        }
    }
    //深入层次遍历
    for (Myfile* temp : this->fileList)
    {
        if (temp->isCatalog())
        {
            Myfile* temp1 = nullptr;
            if ((temp1 = temp->search(fileName, iscatalog)) != nullptr) {
                return temp1;
            }
        }
    }
    cout << "Myfile::search: "
        << this->path
        << "中找不到文件"
        << fileName
        << "!"
        << endl;
    return nullptr;
}

/// <summary>
/// 设置文件内容
/// </summary>
/// <param name="text">文件内容</param>
/// <param name="getb">是否分配磁盘空间</param>
void Myfile::setText(string text, bool getb)
{
    this->text.assign(text);
    if (getb == true)
    {
        return;
    }
    //先清空文件
    delectFile();
    blockList.push_back(disk->get_free_Disk_block());
    int size = text.length();
    //获取磁盘块空间
    size = size / 64;
    for (int i = 0; i < size; i++)
    {
        int dex = blockList.at(0);
        dex = disk->get_free_Disk_block(dex);
        blockList.push_back(dex);
    }
    //写入内容
    writeFileToDisk();
}

/// <summary>
/// 改变文件名
/// </summary>
/// <param name="name">文件名</param>
void Myfile::setName(string name)
{
    Myfile* file = searchFile(name, this->iscatalog);
    if (file != nullptr)
    {
        cout << "Myfile::~setName: "
            << name
            << "文件已经存在！"
            << endl;
    }
    this->name.assign(name);
}

//改变文件路径，实现移动文件功能--------------------------------------------------未完成
void Myfile::changePath(Myfile* parent)
{
    Myfile* file = parent->searchFile(this->name, this->iscatalog);
    if (file != nullptr)
    {
        cout << "Myfile::changePath: 目录"
            << parent->getName()
            << "中已经存在文件"
            << this->name
            << endl;
        return;
    }
    //从现在的目录中移除此文件
    this->parentcatalog->fileList.erase(find(fileList.begin(), fileList.end(), this));
    //设置新的父目录
    this->parentcatalog = parent;
    //在新的父目录中添加此文件
    parent->addFile(this);
}

/// <summary>
/// 改变文件属性
/// </summary>
/// <param name="propertie">文件属性(一个字符，以字节形式存在)</param>
void Myfile::setPropertie(char propertie)
{
    this->propertie = propertie;
}

/// <summary>
/// 设置文件起始盘块号
/// </summary>
/// <param name="dex">起始盘块号</param>
void Myfile::setStartDex(int dex)
{
    this->blockList.clear();
    this->blockList.push_back(dex);
}

/// <summary>
/// 添加文件盘块列表
/// </summary>
/// <param name="dex"></param>
void Myfile::addBlocklist(int dex)
{
    blockList.push_back(dex);
}

/// <summary>
/// 添加子文件
/// </summary>
/// <param name="children">子文件指针</param>
void Myfile::addChildren(Myfile* children)
{
    fileList.push_back(children);
}

/// <summary>
/// 设置父目录
/// </summary>
/// <param name="parentcatalog">父目录指针</param>
void Myfile::setParent(Myfile* parentcatalog)
{
    this->parentcatalog = parentcatalog;
}

/// <summary>
/// 设置路径
/// </summary>
/// <param name="path"></param>
void Myfile::setPath(string path)
{
    this->path = path;
}

/// <summary>
/// 设置是否文件夹
/// </summary>
/// <param name="iscatalog"></param>
void Myfile::setIsCatalog(bool iscatalog)
{
    this->iscatalog = iscatalog;
}

/// <summary>
/// 设置层级
/// </summary>
/// <param name="lev">层级</param>
void Myfile::setLevel(int lev)
{
    this->level = lev;
}

/// <summary>
/// 判断是否文件夹
/// </summary>
/// <returns>是返回true，否返回false</returns>
bool Myfile::isCatalog()
{
    return this->iscatalog;
}

/// <summary>
/// 获取文件名
/// </summary>
/// <returns></returns>
string Myfile::getName()
{
    return this->name;
}

/// <summary>
/// 获取子文件
/// </summary>
/// <param name="index">第几项子文件</param>
/// <returns>相应的文件</returns>
Myfile* Myfile::getChildren(int index)
{
    if (iscatalog == false)
    {
        cout << "Myfile::getChildren: "
            << "这是文件！"
            << endl;
        return nullptr;
    }
    Myfile* file = nullptr;
    if (this->fileList.size() != 0)
    {
        file = this->fileList.at(index);
    }
    else
    {
        //cout << "Myfile::getChildren: "<< "该目录不存在第" << index << "项!"<< endl;
        return nullptr;
    }
    return file;
}

/// <summary>
/// 获取父目录
/// </summary>
/// <returns>父目录的文件指针</returns>
Myfile* Myfile::getParent()
{
    if (parentcatalog == nullptr)
    {
        cout << "Myfile::getParent: "
            << "这是根目录！"
            << endl;
    }
    return this->parentcatalog;
}

/// <summary>
/// 获取开始磁盘文件块号
/// </summary>
/// <returns>开始的磁盘文件块号</returns>
int Myfile::getStartDex()
{
    return blockList[0];
}

/// <summary>
/// 获取占用盘块数量
/// </summary>
/// <returns>占用盘块数量</returns>
int Myfile::getBlockCount()
{
    return blockList.size();
}

/// <summary>
/// 获取盘块列表
/// </summary>
/// <returns>盘块列表</returns>
vector<int> Myfile::getBlockList()
{
    return blockList;
}

/// <summary>
/// 获取文件内容
/// </summary>
/// <returns>文件内容</returns>
string Myfile::getText()
{
    return this->text;
}

/// <summary>
/// 获取文件字节数
/// </summary>
/// <returns>文件字节数</returns>
int Myfile::getSize()
{
    return text.length();
}

/// <summary>
/// 获取文件字节数
/// </summary>
/// <returns>文件字节数</returns>
char Myfile::getPropertie()
{
    return propertie;
}

/// <summary>
/// 获取文件的路径信息
/// </summary>
/// <returns>文件的路径信息</returns>
string Myfile::getPath()
{
    return this->path;
}

/// <summary>
/// 获取层级信息(树形目录中相对的层级)
/// </summary>
/// <returns>该文件是第几层</returns>
int Myfile::getLevel()
{
    return this->level;
}

/// <summary>
/// 添加文件
/// </summary>
/// <param name="file">要添加的文件的指针</param>
/// <returns>是否添加成功</returns>
bool Myfile::addFile(Myfile* file)
{
    if (this->iscatalog == false)
    {
        cout << "Myfile::addFile: "
            << "这是文件！"
            << endl;
        return false;
    }
    //在文件列表中添加此文件
    this->fileList.push_back(file);
    //若文件夹磁盘块已满，添加磁盘块
    if (this->fileList.size() % 8 == 7)
    {
        int dex = disk->get_free_Disk_block(blockList.at(blockList.size() - 1));
        this->blockList.push_back(dex);
    }
    cout << "Myfile::addFile:"
        << file->getName()
        << "文件添加到目录"
        << this->name
        << endl;
    return true;
}

/// <summary>
/// 清空文件空间占用(清空自己和子文件）
/// </summary>
void Myfile::delectFile()
{
    //根目录不可删除
    if (parentcatalog == nullptr)
    {
        cout << "Myfile::delectFile: "
            << "这是根目录！"
            << endl;
        return;
    }
    //腾出磁盘空间
    for (unsigned int i = 0; i < blockList.size(); i++)
    {
        this->disk->change_FAT(this->blockList[i], 0);
    }
    //清空列表,再获取磁盘块
    blockList.clear();
    //blockList.push_back(disk->get_free_Disk_block());
    //如果是文件夹，则递归删除子文件
    if (this->iscatalog)
    {
        for (auto file : fileList)
        {
            file->delectFile();
        }
        fileList.clear();
    }
    cout << "Myfile::delectFile: "
        << name
        << "文件删除成功！"
        << endl;
}

/// <summary>
/// 删除子文件
/// </summary>
/// <param name="children"></param>
void Myfile::deleteChildren(Myfile* children)
{
    auto beg = fileList.begin();
    auto end = fileList.end();
    auto it = beg;
    for (it = beg; it != end; ++it)
    {
        if ((*it) == children)
        {
            break;
        }
    }
    if (it != end)
    {
        fileList.erase(it);
        if (fileList.size() % 8 == 6)
        {
            int dex = blockList.at(blockList.size() - 1);
            blockList.pop_back();
            this->disk->change_FAT(dex, 0);
        }
    }
}

/// <summary>
/// </summary>
void Myfile::writeFileToDisk()
{
    int dex = this->blockList.at(0);
    char buffer[64];
    //遍历文件所有字节
    int size = text.length();
    for (int i = 0; i < size; i++)
    {
        buffer[i % 64] = text[i];
        //读完最后一个直接，写回磁盘
        if (i == size - 1)
        {
            //加结束符
            if (i % 64 != 63)
            {
                buffer[i % 64 + 1] = '\0';
            }
            else
            {
                buffer[63] = '\0';
            }
            disk->write_Block(dex, buffer);
            dex = disk->get_FAT(dex);
            break;
        }
        //buffer2 已满写回磁盘
        if (i % 64 == 63)
        {
            disk->write_Block(dex, buffer);
            dex = disk->get_FAT(dex);
        }
    }
    //生成目录项
    if (parentcatalog != nullptr)
    {
        //找出所在的目录位置
        int pos = 0;
        for (Myfile* f_myfile : parentcatalog->getChildrens())
        {
            if (name.compare(f_myfile->name) == 0 && f_myfile->isCatalog() == iscatalog)
            {
                break;
            }
            pos++;
        }
        //目录项所在盘块号
        int dex = pos / 8;
        pos = pos % 8;
        //计算在磁盘中的位置
        dex = parentcatalog->getBlockList()[dex] * BLOCKSIZE;
        pos = pos * 8;
        pos += dex;
        //生成目录项
        char cat[8] = { 0 };
        int len = name.length();
        for (int j = 0; j < len && j < 5; j++)
        {
            cat[j] = name[j];
        }
        cat[5] = propertie;
        cat[6] = blockList[0];
        cat[7] = text.length();
        fseek(disk->fp, pos, SEEK_SET);
        fwrite(cat, sizeof(cat), 1, disk->fp);
        disk->saveDisk();
        //------------------------------------------------------------------------------------------------------------------
    }
    //cout << "Myfile::writeFileToDisk: "<< text<< "内容写入成功！"<< endl;
}
