#include "heads.h"

int findimg(char *path,struct list **head)
{
    char *ptr = path + strlen(path) - 1;
    if(*ptr != '/')
    {
        *(++ptr) = '/';
        *(++ptr) = '\0';
    }
    DIR *d = opendir(path);
    struct dirent * dir = readdir(d);
	struct stat  fileatt;
    while(dir)
	{
        int len = strlen(dir -> d_name);
		char pathname[1024];
		memset(pathname, 0, 1024);
		strncpy(pathname, path, sizeof pathname);
		strcat(pathname,dir -> d_name);
		lstat(pathname,&fileatt);
        
        if(dir -> d_name[0] != '.' && 
        S_ISREG(fileatt.st_mode) && 
        (strcmp(&dir -> d_name[len - 4],".bmp")==0 || 
        strcmp(&dir -> d_name[len - 4],".jpg")==0 || 
        strcmp(&dir -> d_name[len - 5],".jpeg")==0))
        {
            insertNode(head, pathname);
            //printf("%s\n", pathname);
        }
        else if(dir -> d_name[0] != '.' && S_ISDIR(fileatt.st_mode))
		{
            findimg(pathname,head);
		}
        
        dir = readdir(d);
    }
    closedir(d);
    return 0;
}