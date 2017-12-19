//显示文件列表
#include<unistd.h>
#include<dirent.h>
DIR *dir;
struct dirent *ptr;
dir=opendir("dirPath");
ptr=readdir(dir);
ptr->d_name;

//删除文件
//remove(filename);