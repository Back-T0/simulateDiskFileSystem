
#include "loadFromDisk.h"

/// <summary>
/// ����������Ŀ¼����ļ�
/// </summary>
/// <param name="a">Ŀ¼��</param>
void DiskUtil::creatFile(char a[8])
{
    //��ȡ�ļ���
    char name[6];
    for (int i = 0; i < 5; i++) {
        name[i] = a[i];
    }
    name[5] = '\0';
    string fileName = name;
    //�����ļ�
    Myfile* file = new Myfile(disk1, fileName);
    //�����ļ�����
    file->setPropertie(a[5]);
    //�����Ƿ��ļ���
    if (a[5] == 0b00001100)
    {
        file->setIsCatalog(true);
    }
    else
    {
        file->setIsCatalog(false);
    }
    //������ʼ�̿��
    int startBlock = a[6];
    file->setStartDex(startBlock);
    //��ȡ�ļ�����
    string content = disk1->read_Block(startBlock);
    //����FAT����������ļ����ݲ�ֹһ�����̿飬׷���ļ�
    int dex = disk1->get_FAT(startBlock);
    while (dex != -1 && dex != 0)
    {
        content.append(disk1->read_Block(dex));
        file->addBlocklist(dex);
        dex = disk1->get_FAT(dex);
    }
    //�����ļ�����
    file->setText(content, true);
    //�����ļ���Ŀ¼
    file->setParent(current);
    //�ڸ�Ŀ¼����Ӹ��ļ�
    current->addChildren(file);

    //����·��
    string path = current->getPath();
    path.append(fileName);
    path.append("/");
    file->setPath(path);
    //���ò㼶
    file->setLevel(current->getLevel() + 1);
}

/// <summary>
/// �����̿��е�Ŀ¼������������ļ�
/// </summary>
/// <param name="a">Ŀ¼�̿�����</param>
void DiskUtil::traverseDirEntries(int dex)
{
    //��ȡ�̿�
    char a[64];
    disk1->read_Block(dex, a);
    char temp[8];
    //��������Ŀ¼��
    for (int i = 0; i < 64; i += 8)
    {
        //���Ŀ¼��ǿ�
        if (a[i] != '$' && a[i] != 0)
        {
            //����Ŀ¼��
            for (int j = 0; j < 8; j++)
            {
                temp[j] = a[i + j];
            }
            //�����ļ�
            creatFile(temp);
        }
    }
    //����FAT���������Ŀ¼��ֹһ���̿飬��ȡ��һ���̿��Ŀ¼��
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
/// �������ļ��У������ļ����е�Ŀ¼����ļ�
/// </summary>
/// <param name="file">�ļ������</param>
void DiskUtil::loadCatalog(Myfile* file)
{
    //�����������ļ���
    for (auto children : file->getChildrens())
    {
        if (children->isCatalog())
        {
            //������ļ��У�������ļ����е�Ŀ¼����ļ�
            current = children;
            traverseDirEntries(children->getStartDex());
            //�ݹ��ļ���
            loadCatalog(children);
        }
    }
}

/// <summary>
/// �Ӵ�����������״�ļ���
/// </summary>
/// <returns>�ļ���Ŀ¼</returns>
Myfile* DiskUtil::loadDisk()
{
    //�����ɸ�Ŀ¼�����ļ�
    traverseDirEntries(2);
    //������Ŀ¼���ļ����ݹ����������ļ�
    loadCatalog(root1);
    return root1;
}


