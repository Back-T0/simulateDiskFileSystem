# 一、需求分析

##  1.课程设计目的

了解磁盘文件系统的结构、功能和实现，设计一个简单的文件系统，用文件模拟磁盘，用数组模拟缓冲区。

##  2.现有系统环境

###  (1)硬件环境

处理器：AMDRyzen5

内存：16.0GB

硬盘空间：512GB

###  (2)软件环境

操作系统：windows11

IDE：VisualStudio2019

语言：C++

##  3.输入的形式和输入值的范围

输入形式：操作界面输入\'w\'进行上移选项，输入s进行下移选项，输入\'a\'进行返回操作，输入\'d\'进行打开操作，输入回车进入相应的选项，输入其他刷新界面。

输入值的范围：新建文件的文件名不超过五个字节（检测超过将提示）。

##  4.输出的形式

图形化显示。

##  5.程序达到的功能

###  (1)基本功能

支持多级目录结构，支持文件的绝对读路径；

文件的逻辑结构采用流式结构，物理结构采用链接结构中的显式链接方式；

采用文件分配表FAT

实现的命令包括建立目录、列目录、删除空目录、建立文件、删除文件、显示文件内容、打开文件、读文件、写文件、关闭文件、改变文件属性。可以采用命令行界面执行这些命令，也可以采用"右击快捷菜单选择"方式执行命令。

最后编写主函数对所作工作进行测试。

###  (2)附加功能

支持子目录可以任意长（磁盘有可分配磁盘块）

文件内容可以任意长（磁盘有可分配磁盘块）

支持非空文件夹的递归删除

支持全局搜索文件

支持文件自动保存

支持绝对路径

支持读取二进制磁盘文件

支持多级树形目录显示

支持通过已打开文件列表进行读写操作

详细文档请点击[simulateADiskFileSystem.md](https://github.com/Back-T0/simulateDiskFileSystem/blob/master/simulateADiskFileSystem.md)。
