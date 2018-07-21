#ifndef _HEADS_H_
#define _HEADS_H_

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <linux/input.h>
#include "jpeglib.h"

#define TOUCH_DEV "/dev/input/event0"
/* *
 * 双向循环链表的节点声明
 * */
struct list
{
	char path[1024];
	struct list *next;
	struct list *prev;
};

/* *
 * 屏幕操作返回值的定义
 * */
enum touch_num
{
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	GO_LEFT,
	GO_RIGHT,
	GO_UP,
	GO_DOWN,
};

/* *
 * 屏幕映射内存指针
 * */
int *fbuff;

/* *
 * 屏幕大小
 * */
int X, Y;

extern int getDirection(void);

void drawpoint(int x, int y, int rgb);
void showjpeg(FILE *fp, double ffector, int xmove, int ymove);
void showbmp(int fd, double ffector, int xmove, int ymove);
int insertNode(struct list **head, char *path);
int findimg(char *path, struct list **head);

#endif