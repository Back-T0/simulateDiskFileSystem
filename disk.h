
#ifndef DISK_H
#define DISK_H

#include <iostream>
using namespace std;
#define BLOCKSIZE 64
#define BLOCKCOUNT 128

class DISK
{
public:
    DISK();
    DISK(char filePath[128]);
    ~DISK();

    //对FAT操作

    void change_FAT(int dex, int next);
    int get_FAT(int dex);
    int get_free_Disk_block();
    int get_free_Disk_block(int dex);
    void updata_FAT_to_Disk();

    //磁盘读写操作

    std::string read_Block(int dex);
    void read_Block(int dex, char* content);
    void write_Block(int dex, char content[64]);
    void initDisk();
    void saveDisk();
    void closeDISK();

    FILE* fp; //磁盘(为了方便测试的时候读取文件，将文件设为 public，停止测试后设为 private)

private:
    char FAT[BLOCKCOUNT]; //FAT
    char buffer1[BLOCKSIZE] = { 0 }; //缓冲1
    char buffer2[BLOCKSIZE] = { 0 }; //缓冲2
    int blockNumber1; //当前buffer1所缓存 的 磁盘盘块号
    int blockNumber2; //当前buffer2所缓存 的 磁盘盘块号

    //使用buffer对磁盘进行读写操作

    void load_Disk_block_into_buffer1(int dex);
    void load_Disk_block_into_buffer2(int dex);
    void write_buffer1_into_Disk(int dex);
    void write_buffer2_into_Disk(int dex);
};

#endif // DISK_H


