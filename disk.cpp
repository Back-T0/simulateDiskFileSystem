
#include "disk.h"
#pragma warning(disable:4996)
DISK::DISK()
{
    //打开文件
    fp = fopen("Disk", "w+");
    //初始化磁盘
    initDisk();
}

DISK::DISK(char filePath[128])
{
    fp = fopen(filePath, "r+");
    fread(FAT, sizeof(FAT), 1, (FILE*)fp);
}

DISK::~DISK()
{
    closeDISK();
    delete fp;
}

/// <summary>
/// 初始化FAT，根目录，并写入磁盘
/// </summary>
void DISK::initDisk()
{
    int i = 128;
    while (i--)
    {
        fwrite(buffer2, sizeof(buffer2), 1, fp);
    }
    //初始化FAT
    change_FAT(0, -1);
    change_FAT(1, -1);
    //根目录初始化为空目录项
    fseek(fp, 128, SEEK_SET);
    char temp[8] = { 0 };
    temp[0] = '$';
    i = 8;
    while (i--)
    {
        fwrite(temp, sizeof(temp), 1, fp);
    }
}

/// <summary>
/// 将磁盘文件关闭，再以"r+"方式打开，达到保存的目的
/// </summary>
void DISK::saveDisk()
{
    fclose(fp);
    fp = fopen("Disk", "r+");
}

/// <summary>
/// 更新FAT到磁盘中，然后关闭文件
/// </summary>
void DISK::closeDISK()
{
    updata_FAT_to_Disk();
    fclose(fp);
}

/// <summary>
/// 改变FAT
/// </summary>
/// <param name="dex">将要改变的FAT</param>
/// <param name="next">将要改成的FAT值</param>
void DISK::change_FAT(int dex, int next)
{
    FAT[dex] = next;
    //cout << "DISK::change_FAT(int dex, int next): FAT[" << dex << "]已经修改为" << next << endl;
}

/// <summary>
/// 获取FAT值
/// </summary>
/// <param name="dex">要获取的FAT</param>
/// <returns>要获取的FAT值</returns>
int DISK::get_FAT(int dex)
{
    return FAT[dex];
}

/// <summary>
/// 获取空闲磁盘块，并占用
/// </summary>
/// <returns>已获取的空闲磁盘块，获取失败返回 0</returns>
int DISK::get_free_Disk_block()
{
    int i;
    for (i = 2; i < BLOCKCOUNT; i++)
    {
        if (FAT[i] == 0 || FAT[i] == static_cast<char>(254))break;
    }
    if (i == BLOCKCOUNT)
    {
        //cout << "DISK::get_free_Disk_block: 磁盘已经满" << endl;
        return 0;
    }
    change_FAT(i, -1);
    //cout << "DISK::get_free_Disk_block: 查找到空闲块" << i << endl;
    return i;
}

/// <summary>
/// 获取空闲磁盘块，并连接到dex后面
/// </summary>
/// <param name="dex">要连接的磁盘快号</param>
/// <returns>已获取的空闲磁盘块，获取失败返回 0</returns>
int DISK::get_free_Disk_block(int dex)
{
    int i;
    for (i = 2; i < BLOCKCOUNT; i++)
    {
        if (FAT[i] == 0 || FAT[i] == static_cast<char>(254)) {
            break;
        }
    }
    if (i == BLOCKCOUNT)
    {
        //cout << "DISK::get_free_Disk_block: 磁盘已经满" << endl;
        return 0;
    }
    change_FAT(dex, i);
    change_FAT(i, -1);
    //cout << "DISK::get_free_Disk_block: 查找到空闲块" << i << endl;
    return i;
}

/// <summary>
/// 更新FAT信息到磁盘中（改变buffer1）
/// </summary>
void DISK::updata_FAT_to_Disk()
{
    int i;
    for (i = 0; i < BLOCKSIZE; i++)
    {
        buffer1[i] = FAT[i];
    }
    write_buffer1_into_Disk(0);
    for (i = 0; i < BLOCKSIZE; i++)
    {
        buffer1[i] = FAT[i + BLOCKSIZE];
    }
    write_buffer1_into_Disk(1);
}

/// <summary>
/// 给出磁盘块号，装填磁盘块到内存buffer1中
/// </summary>
/// <param name="dex">磁盘块号</param>
void DISK::load_Disk_block_into_buffer1(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fread(buffer1, sizeof(buffer1), 1, fp);
    blockNumber1 = dex;
}

/// <summary>
/// 给出磁盘块号，装填磁盘块到内存buffer2中
/// </summary>
/// <param name="dex">磁盘块号</param>
void DISK::load_Disk_block_into_buffer2(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fread(buffer2, sizeof(buffer2), 1, fp);
    blockNumber2 = dex;
}

/// <summary>
/// 回写buffer1到磁盘中
/// </summary>
/// <param name="dex">要回写到哪个磁盘块</param>
void DISK::write_buffer1_into_Disk(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fwrite(buffer1, sizeof(buffer1), 1, fp);
    saveDisk();
    blockNumber1 = dex;
}

/// <summary>
/// 回写buffer2到磁盘中
/// </summary>
/// <param name="dex">要回写到哪个磁盘块</param>
void DISK::write_buffer2_into_Disk(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fwrite(buffer2, sizeof(buffer2), 1, fp);
    saveDisk();
    blockNumber2 = dex;
}

/// <summary>
/// 磁盘读操作
/// </summary>
/// <param name="dex">要读取的磁盘块号</param>
/// <returns>读取出的数据</returns>
string DISK::read_Block(int dex)
{
    string str;
    load_Disk_block_into_buffer1(dex);
    str.append(buffer1);
    return str;
}

void DISK::read_Block(int dex, char* content)
{
    load_Disk_block_into_buffer1(dex);
    for (int i = 0; i < BLOCKSIZE; i++)
    {
        content[i] = buffer1[i];
    }
}

/// <summary>
/// 磁盘写操作
/// </summary>
/// <param name="dex">要写入到哪个磁盘块</param>
/// <param name="content">要写入的内容</param>
void DISK::write_Block(int dex, char content[64])
{
    int i;
    for (i = 0; i < BLOCKSIZE; i++)
    {
        buffer1[i] = content[i];
    }
    while (i < BLOCKSIZE)
    {
        buffer1[i] = '\0';
    }
    write_buffer1_into_Disk(dex);
}