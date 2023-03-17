
#include "loadFromDisk.h"

/// <summary>
/// 根据所给的目录项创建文件
/// </summary>
/// <param name="a">目录项</param>
void DiskUtil::creatFile(char a[8])
{
    //获取文件名
    char name[6];
    for (int i = 0; i < 5; i++) {
        name[i] = a[i];
    }
    name[5] = '\0';
    string fileName = name;
    //创建文件
    Myfile* file = new Myfile(disk1, fileName);
    //设置文件属性
    file->setPropertie(a[5]);
    //设置是否文件夹
    if (a[5] == 0b00001100)
    {
        file->setIsCatalog(true);
    }
    else
    {
        file->setIsCatalog(false);
    }
    //设置起始盘块号
    int startBlock = a[6];
    file->setStartDex(startBlock);
    //读取文件内容
    string content = disk1->read_Block(startBlock);
    //根据FAT索引，如果文件内容不止一个磁盘块，追加文件
    int dex = disk1->get_FAT(startBlock);
    while (dex != -1 && dex != 0)
    {
        content.append(disk1->read_Block(dex));
        file->addBlocklist(dex);
        dex = disk1->get_FAT(dex);
    }
    //设置文件内容
    file->setText(content, true);
    //设置文件父目录
    file->setParent(current);
    //在父目录中添加该文件
    current->addChildren(file);

    //设置路径
    string path = current->getPath();
    path.append(fileName);
    path.append("/");
    file->setPath(path);
    //设置层级
    file->setLevel(current->getLevel() + 1);
}

/// <summary>
/// 根据盘块中的目录项逐个生成子文件
/// </summary>
/// <param name="a">目录盘块内容</param>
void DiskUtil::traverseDirEntries(int dex)
{
    //读取盘块
    char a[64];
    disk1->read_Block(dex, a);
    char temp[8];
    //遍历所有目录项
    for (int i = 0; i < 64; i += 8)
    {
        //如果目录项非空
        if (a[i] != '$' && a[i] != 0)
        {
            //复制目录项
            for (int j = 0; j < 8; j++)
            {
                temp[j] = a[i + j];
            }
            //生成文件
            creatFile(temp);
        }
    }
    //根据FAT索引，如果目录不止一个盘块，读取下一个盘块的目录项
    int pos = dex;
    while ((pos = disk1->get_FAT(pos)) != -1)
    {
        if (pos == 0)
        {
            break;
        }
        traverseDirEntries(pos);
    }
}

/// <summary>
/// 遍历子文件夹，从子文件夹中的目录项创建文件
/// </summary>
/// <param name="file">文件夹入口</param>
void DiskUtil::loadCatalog(Myfile* file)
{
    //遍历所有子文件夹
    for (auto children : file->getChildrens())
    {
        if (children->isCatalog())
        {
            //如果是文件夹，则根据文件夹中的目录项创建文件
            current = children;
            traverseDirEntries(children->getStartDex());
            //递归文件夹
            loadCatalog(children);
        }
    }
}

/// <summary>
/// 从磁盘中生成树状文件串
/// </summary>
/// <returns>文件根目录</returns>
Myfile* DiskUtil::loadDisk()
{
    //先生成根目录的子文件
    traverseDirEntries(2);
    //遍历根目录子文件，递归生成子子文件
    loadCatalog(root1);
    return root1;
}


