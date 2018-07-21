#include "heads.h"

int main(int argc, char **argv)
{
    char path[1024];

    if (argc > 2)
    {
        fprintf(stderr, "USEAGE: %s [path]\n", argv[0]);
    }
    else if (argc == 2)
    {
        strncpy(path, argv[1], strlen(argv[1]) + 1);
    }
    else
    {
        strncpy(path, "./", strlen("./"));
    }

    struct list *head = NULL;
    findimg(path, &head);

    int framefd = open("/dev/fb0", O_RDWR);
    if (framefd == -1)
    {
        perror("open device error");
        return -1;
    }

    struct fb_var_screeninfo buf;
    ioctl(framefd, FBIOGET_VSCREENINFO, &buf);
    fbuff = mmap(NULL, buf.xres * buf.yres * buf.bits_per_pixel / 8, PROT_READ | PROT_WRITE, MAP_SHARED, framefd, 0);
    X = buf.xres;
    Y = buf.yres;

    double fector = 1;
    int xoff = 0;
    int yoff = 0;

    while (1)
    {
        int len = strlen(head->path);
        if (strcmp(&head->path[len - 4], ".bmp") == 0)
        {
            int fd = open(head->path, O_RDONLY);
            if (fd == -1)
            {
                perror("open error");
                fprintf(stderr, "%s\n", head->path);
            }
            showbmp(fd, fector, xoff, yoff);
            close(fd);
        }
        else
        {
            FILE *fp = fopen(head->path, "r");
            if (fp == NULL)
            {
                perror("open error");
                fprintf(stderr, "%s\n", head->path);
            }
            showjpeg(fp, fector, xoff, yoff);
            fclose(fp);
        }
        //        fprintf(stderr, "Line %d\n", __LINE__);
        int dire = getDirection();
        //        fprintf(stderr, "Line %d dir = %d \n", __LINE__, dire);
        if (dire == MOVE_RIGHT)
        {
            head = head->prev;
            fector = 1;
            xoff = 0;
            yoff = 0;
        }
        else if (dire == MOVE_LEFT)
        {
            head = head->next;
            fector = 1;
            xoff = 0;
            yoff = 0;
        }
        else if (dire == MOVE_DOWN)
            fector *= 2;
        else if (dire == MOVE_UP)
            fector *= 0.5;
        else if (dire == GO_DOWN)
            yoff -= 30;
        else if (dire == GO_UP)
            yoff += 30;
        else if (dire == GO_LEFT)
            xoff += 50;
        else if (dire == GO_RIGHT)
            xoff -= 50;
    }
    return 0;
}