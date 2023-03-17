
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

    //��FAT����

    void change_FAT(int dex, int next);
    int get_FAT(int dex);
    int get_free_Disk_block();
    int get_free_Disk_block(int dex);
    void updata_FAT_to_Disk();

    //���̶�д����

    std::string read_Block(int dex);
    void read_Block(int dex, char* content);
    void write_Block(int dex, char content[64]);
    void initDisk();
    void saveDisk();
    void closeDISK();

    FILE* fp; //����(Ϊ�˷�����Ե�ʱ���ȡ�ļ������ļ���Ϊ public��ֹͣ���Ժ���Ϊ private)

private:
    char FAT[BLOCKCOUNT]; //FAT
    char buffer1[BLOCKSIZE] = { 0 }; //����1
    char buffer2[BLOCKSIZE] = { 0 }; //����2
    int blockNumber1; //��ǰbuffer1������ �� �����̿��
    int blockNumber2; //��ǰbuffer2������ �� �����̿��

    //ʹ��buffer�Դ��̽��ж�д����

    void load_Disk_block_into_buffer1(int dex);
    void load_Disk_block_into_buffer2(int dex);
    void write_buffer1_into_Disk(int dex);
    void write_buffer2_into_Disk(int dex);
};

#endif // DISK_H


