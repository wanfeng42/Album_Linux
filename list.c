#include "heads.h"
/* *
 * 双向循环链表的初始化和插入；
 * 参数: 
 *  head: 指向头指针的指针。
 *  path: 字符串，路径。
 * */
int insertNode(struct list **head, char *path)
{
    if (path == NULL)
        return -1;
    struct list *tail;
    struct list *p = malloc(sizeof(*p));
    strncpy(p->path, path, strlen(path) + 1);
    if ((*head) == NULL)
    {
        (*head) = p;
        (*head)->next = p;
        (*head)->prev = p;
        return 0;
    }

    tail = (*head)->prev;
    tail->next = p;
    p->prev = tail;
    p->next = NULL;
    tail = p;
    tail->next = (*head);
    (*head)->prev = tail;
    return 0;
}
