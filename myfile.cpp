
#include "myfile.h"
#include <algorithm>
#pragma warning(disable:4996)

/// <summary>
/// ���캯��
/// </summary>
/// <param name="disk">����ָ��</param>
/// <param name="fileName">Ҫ�������ļ���</param>
/// <param name="iscatalog">�Ƿ��ļ���</param>
/// <param name="catalog">��Ŀ¼�ļ�ָ��</param>
Myfile::Myfile(DISK* disk, string fileName, bool iscatalog, Myfile* catalog)
{
    //������̿�
    this->disk = disk;
    int startBlock = disk->get_free_Disk_block();
    blockList.push_back(startBlock);

    //���峣������
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
    //cout << "Myfile::Myfile: "<< this->name << "�����ɹ���"<< endl;
}

/// <summary>
/// ���캯��������ֻ���ļ����ĵ��ļ�����Ҫ�������ò���ʹ��
/// </summary>
/// <param name="disk">����</param>
/// <param name="fileName">�ļ���</param>
Myfile::Myfile(DISK* disk, string fileName) :
    disk(disk), name(fileName)
{
    //������̿�
    this->disk = disk;

    //���峣������
    this->iscatalog = true;
    this->name = fileName;
    this->level = 0;
    this->path.append(":");
    this->path.append(fileName);
    this->path.append("/");
    propertie = 0b00001100;
}

/// <summary>
/// ��������
/// </summary>
Myfile::~Myfile()
{
    fileList.clear();
    cout << "Myfile::~Myfile: "
        << this->name
        << "�ļ�������"
        << endl;
}

/// <summary>
/// ������Ŀ¼�������ļ���
/// </summary>
/// <param name="fileName">Ҫ���ҵ��ļ���</param>
/// <param name="iscatalog">�Ƿ��ļ���</param>
/// <returns></returns>
Myfile* Myfile::searchFile(string fileName, bool iscatalog)
{
    //����Ŀ¼���Ƿ����д��ļ�
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
    //����������ļ����Ӹ�Ŀ¼������
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
        << "���Ҳ����ļ�"
        << fileName
        << "!"
        << endl;
    return nullptr;
}

/// <summary>
/// ���������ļ��������ļ���
/// </summary>
/// <param name="fileName">Ҫ���ҵ��ļ���</param>
/// <param name="iscatalog">�Ƿ��ļ���</param>
/// <returns></returns>
Myfile* Myfile::search(string fileName, bool iscatalog)
{
    //����Ŀ¼���Ƿ����д��ļ�
    for (Myfile* temp : this->fileList)
    {
        if (fileName.compare(temp->name) == 0 && (temp->iscatalog == iscatalog))
        {
            return temp;
        }
    }
    //�����α���
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
        << "���Ҳ����ļ�"
        << fileName
        << "!"
        << endl;
    return nullptr;
}

/// <summary>
/// �����ļ�����
/// </summary>
/// <param name="text">�ļ�����</param>
/// <param name="getb">�Ƿ������̿ռ�</param>
void Myfile::setText(string text, bool getb)
{
    this->text.assign(text);
    if (getb == true)
    {
        return;
    }
    //������ļ�
    delectFile();
    blockList.push_back(disk->get_free_Disk_block());
    int size = text.length();
    //��ȡ���̿�ռ�
    size = size / 64;
    for (int i = 0; i < size; i++)
    {
        int dex = blockList.at(0);
        dex = disk->get_free_Disk_block(dex);
        blockList.push_back(dex);
    }
    //д������
    writeFileToDisk();
}

/// <summary>
/// �ı��ļ���
/// </summary>
/// <param name="name">�ļ���</param>
void Myfile::setName(string name)
{
    Myfile* file = searchFile(name, this->iscatalog);
    if (file != nullptr)
    {
        cout << "Myfile::~setName: "
            << name
            << "�ļ��Ѿ����ڣ�"
            << endl;
    }
    this->name.assign(name);
}

//�ı��ļ�·����ʵ���ƶ��ļ�����--------------------------------------------------δ���
void Myfile::changePath(Myfile* parent)
{
    Myfile* file = parent->searchFile(this->name, this->iscatalog);
    if (file != nullptr)
    {
        cout << "Myfile::changePath: Ŀ¼"
            << parent->getName()
            << "���Ѿ������ļ�"
            << this->name
            << endl;
        return;
    }
    //�����ڵ�Ŀ¼���Ƴ����ļ�
    this->parentcatalog->fileList.erase(find(fileList.begin(), fileList.end(), this));
    //�����µĸ�Ŀ¼
    this->parentcatalog = parent;
    //���µĸ�Ŀ¼����Ӵ��ļ�
    parent->addFile(this);
}

/// <summary>
/// �ı��ļ�����
/// </summary>
/// <param name="propertie">�ļ�����(һ���ַ������ֽ���ʽ����)</param>
void Myfile::setPropertie(char propertie)
{
    this->propertie = propertie;
}

/// <summary>
/// �����ļ���ʼ�̿��
/// </summary>
/// <param name="dex">��ʼ�̿��</param>
void Myfile::setStartDex(int dex)
{
    this->blockList.clear();
    this->blockList.push_back(dex);
}

/// <summary>
/// ����ļ��̿��б�
/// </summary>
/// <param name="dex"></param>
void Myfile::addBlocklist(int dex)
{
    blockList.push_back(dex);
}

/// <summary>
/// ������ļ�
/// </summary>
/// <param name="children">���ļ�ָ��</param>
void Myfile::addChildren(Myfile* children)
{
    fileList.push_back(children);
}

/// <summary>
/// ���ø�Ŀ¼
/// </summary>
/// <param name="parentcatalog">��Ŀ¼ָ��</param>
void Myfile::setParent(Myfile* parentcatalog)
{
    this->parentcatalog = parentcatalog;
}

/// <summary>
/// ����·��
/// </summary>
/// <param name="path"></param>
void Myfile::setPath(string path)
{
    this->path = path;
}

/// <summary>
/// �����Ƿ��ļ���
/// </summary>
/// <param name="iscatalog"></param>
void Myfile::setIsCatalog(bool iscatalog)
{
    this->iscatalog = iscatalog;
}

/// <summary>
/// ���ò㼶
/// </summary>
/// <param name="lev">�㼶</param>
void Myfile::setLevel(int lev)
{
    this->level = lev;
}

/// <summary>
/// �ж��Ƿ��ļ���
/// </summary>
/// <returns>�Ƿ���true���񷵻�false</returns>
bool Myfile::isCatalog()
{
    return this->iscatalog;
}

/// <summary>
/// ��ȡ�ļ���
/// </summary>
/// <returns></returns>
string Myfile::getName()
{
    return this->name;
}

/// <summary>
/// ��ȡ���ļ�
/// </summary>
/// <param name="index">�ڼ������ļ�</param>
/// <returns>��Ӧ���ļ�</returns>
Myfile* Myfile::getChildren(int index)
{
    if (iscatalog == false)
    {
        cout << "Myfile::getChildren: "
            << "�����ļ���"
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
        //cout << "Myfile::getChildren: "<< "��Ŀ¼�����ڵ�" << index << "��!"<< endl;
        return nullptr;
    }
    return file;
}

/// <summary>
/// ��ȡ��Ŀ¼
/// </summary>
/// <returns>��Ŀ¼���ļ�ָ��</returns>
Myfile* Myfile::getParent()
{
    if (parentcatalog == nullptr)
    {
        cout << "Myfile::getParent: "
            << "���Ǹ�Ŀ¼��"
            << endl;
    }
    return this->parentcatalog;
}

/// <summary>
/// ��ȡ��ʼ�����ļ����
/// </summary>
/// <returns>��ʼ�Ĵ����ļ����</returns>
int Myfile::getStartDex()
{
    return blockList[0];
}

/// <summary>
/// ��ȡռ���̿�����
/// </summary>
/// <returns>ռ���̿�����</returns>
int Myfile::getBlockCount()
{
    return blockList.size();
}

/// <summary>
/// ��ȡ�̿��б�
/// </summary>
/// <returns>�̿��б�</returns>
vector<int> Myfile::getBlockList()
{
    return blockList;
}

/// <summary>
/// ��ȡ�ļ�����
/// </summary>
/// <returns>�ļ�����</returns>
string Myfile::getText()
{
    return this->text;
}

/// <summary>
/// ��ȡ�ļ��ֽ���
/// </summary>
/// <returns>�ļ��ֽ���</returns>
int Myfile::getSize()
{
    return text.length();
}

/// <summary>
/// ��ȡ�ļ��ֽ���
/// </summary>
/// <returns>�ļ��ֽ���</returns>
char Myfile::getPropertie()
{
    return propertie;
}

/// <summary>
/// ��ȡ�ļ���·����Ϣ
/// </summary>
/// <returns>�ļ���·����Ϣ</returns>
string Myfile::getPath()
{
    return this->path;
}

/// <summary>
/// ��ȡ�㼶��Ϣ(����Ŀ¼����ԵĲ㼶)
/// </summary>
/// <returns>���ļ��ǵڼ���</returns>
int Myfile::getLevel()
{
    return this->level;
}

/// <summary>
/// ����ļ�
/// </summary>
/// <param name="file">Ҫ��ӵ��ļ���ָ��</param>
/// <returns>�Ƿ���ӳɹ�</returns>
bool Myfile::addFile(Myfile* file)
{
    if (this->iscatalog == false)
    {
        cout << "Myfile::addFile: "
            << "�����ļ���"
            << endl;
        return false;
    }
    //���ļ��б�����Ӵ��ļ�
    this->fileList.push_back(file);
    //���ļ��д��̿���������Ӵ��̿�
    if (this->fileList.size() % 8 == 7)
    {
        int dex = disk->get_free_Disk_block(blockList.at(blockList.size() - 1));
        this->blockList.push_back(dex);
    }
    cout << "Myfile::addFile:"
        << file->getName()
        << "�ļ���ӵ�Ŀ¼"
        << this->name
        << endl;
    return true;
}

/// <summary>
/// ����ļ��ռ�ռ��(����Լ������ļ���
/// </summary>
void Myfile::delectFile()
{
    //��Ŀ¼����ɾ��
    if (parentcatalog == nullptr)
    {
        cout << "Myfile::delectFile: "
            << "���Ǹ�Ŀ¼��"
            << endl;
        return;
    }
    //�ڳ����̿ռ�
    for (unsigned int i = 0; i < blockList.size(); i++)
    {
        this->disk->change_FAT(this->blockList[i], 0);
    }
    //����б�,�ٻ�ȡ���̿�
    blockList.clear();
    //blockList.push_back(disk->get_free_Disk_block());
    //������ļ��У���ݹ�ɾ�����ļ�
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
        << "�ļ�ɾ���ɹ���"
        << endl;
}

/// <summary>
/// ɾ�����ļ�
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
    //�����ļ������ֽ�
    int size = text.length();
    for (int i = 0; i < size; i++)
    {
        buffer[i % 64] = text[i];
        //�������һ��ֱ�ӣ�д�ش���
        if (i == size - 1)
        {
            //�ӽ�����
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
        //buffer2 ����д�ش���
        if (i % 64 == 63)
        {
            disk->write_Block(dex, buffer);
            dex = disk->get_FAT(dex);
        }
    }
    //����Ŀ¼��
    if (parentcatalog != nullptr)
    {
        //�ҳ����ڵ�Ŀ¼λ��
        int pos = 0;
        for (Myfile* f_myfile : parentcatalog->getChildrens())
        {
            if (name.compare(f_myfile->name) == 0 && f_myfile->isCatalog() == iscatalog)
            {
                break;
            }
            pos++;
        }
        //Ŀ¼�������̿��
        int dex = pos / 8;
        pos = pos % 8;
        //�����ڴ����е�λ��
        dex = parentcatalog->getBlockList()[dex] * BLOCKSIZE;
        pos = pos * 8;
        pos += dex;
        //����Ŀ¼��
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
    //cout << "Myfile::writeFileToDisk: "<< text<< "����д��ɹ���"<< endl;
}
