
#include "disk.h"
#pragma warning(disable:4996)
DISK::DISK()
{
    //���ļ�
    fp = fopen("Disk", "w+");
    //��ʼ������
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
/// ��ʼ��FAT����Ŀ¼����д�����
/// </summary>
void DISK::initDisk()
{
    int i = 128;
    while (i--)
    {
        fwrite(buffer2, sizeof(buffer2), 1, fp);
    }
    //��ʼ��FAT
    change_FAT(0, -1);
    change_FAT(1, -1);
    //��Ŀ¼��ʼ��Ϊ��Ŀ¼��
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
/// �������ļ��رգ�����"r+"��ʽ�򿪣��ﵽ�����Ŀ��
/// </summary>
void DISK::saveDisk()
{
    fclose(fp);
    fp = fopen("Disk", "r+");
}

/// <summary>
/// ����FAT�������У�Ȼ��ر��ļ�
/// </summary>
void DISK::closeDISK()
{
    updata_FAT_to_Disk();
    fclose(fp);
}

/// <summary>
/// �ı�FAT
/// </summary>
/// <param name="dex">��Ҫ�ı��FAT</param>
/// <param name="next">��Ҫ�ĳɵ�FATֵ</param>
void DISK::change_FAT(int dex, int next)
{
    FAT[dex] = next;
    //cout << "DISK::change_FAT(int dex, int next): FAT[" << dex << "]�Ѿ��޸�Ϊ" << next << endl;
}

/// <summary>
/// ��ȡFATֵ
/// </summary>
/// <param name="dex">Ҫ��ȡ��FAT</param>
/// <returns>Ҫ��ȡ��FATֵ</returns>
int DISK::get_FAT(int dex)
{
    return FAT[dex];
}

/// <summary>
/// ��ȡ���д��̿飬��ռ��
/// </summary>
/// <returns>�ѻ�ȡ�Ŀ��д��̿飬��ȡʧ�ܷ��� 0</returns>
int DISK::get_free_Disk_block()
{
    int i;
    for (i = 2; i < BLOCKCOUNT; i++)
    {
        if (FAT[i] == 0 || FAT[i] == static_cast<char>(254))break;
    }
    if (i == BLOCKCOUNT)
    {
        //cout << "DISK::get_free_Disk_block: �����Ѿ���" << endl;
        return 0;
    }
    change_FAT(i, -1);
    //cout << "DISK::get_free_Disk_block: ���ҵ����п�" << i << endl;
    return i;
}

/// <summary>
/// ��ȡ���д��̿飬�����ӵ�dex����
/// </summary>
/// <param name="dex">Ҫ���ӵĴ��̿��</param>
/// <returns>�ѻ�ȡ�Ŀ��д��̿飬��ȡʧ�ܷ��� 0</returns>
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
        //cout << "DISK::get_free_Disk_block: �����Ѿ���" << endl;
        return 0;
    }
    change_FAT(dex, i);
    change_FAT(i, -1);
    //cout << "DISK::get_free_Disk_block: ���ҵ����п�" << i << endl;
    return i;
}

/// <summary>
/// ����FAT��Ϣ�������У��ı�buffer1��
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
/// �������̿�ţ�װ����̿鵽�ڴ�buffer1��
/// </summary>
/// <param name="dex">���̿��</param>
void DISK::load_Disk_block_into_buffer1(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fread(buffer1, sizeof(buffer1), 1, fp);
    blockNumber1 = dex;
}

/// <summary>
/// �������̿�ţ�װ����̿鵽�ڴ�buffer2��
/// </summary>
/// <param name="dex">���̿��</param>
void DISK::load_Disk_block_into_buffer2(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fread(buffer2, sizeof(buffer2), 1, fp);
    blockNumber2 = dex;
}

/// <summary>
/// ��дbuffer1��������
/// </summary>
/// <param name="dex">Ҫ��д���ĸ����̿�</param>
void DISK::write_buffer1_into_Disk(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fwrite(buffer1, sizeof(buffer1), 1, fp);
    saveDisk();
    blockNumber1 = dex;
}

/// <summary>
/// ��дbuffer2��������
/// </summary>
/// <param name="dex">Ҫ��д���ĸ����̿�</param>
void DISK::write_buffer2_into_Disk(int dex)
{
    fseek(fp, dex * BLOCKSIZE, SEEK_SET);
    fwrite(buffer2, sizeof(buffer2), 1, fp);
    saveDisk();
    blockNumber2 = dex;
}

/// <summary>
/// ���̶�����
/// </summary>
/// <param name="dex">Ҫ��ȡ�Ĵ��̿��</param>
/// <returns>��ȡ��������</returns>
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
/// ����д����
/// </summary>
/// <param name="dex">Ҫд�뵽�ĸ����̿�</param>
/// <param name="content">Ҫд�������</param>
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