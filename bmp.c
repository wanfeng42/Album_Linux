#include "heads.h"

extern int *fbuff;

/* * * * *
 * 在指定坐标处画点，其颜色为rgb
 * 参数：x、y为坐标，rgb为颜色
 * * * * */
void drawpoint(int x, int y, int rgb)
{
    fbuff[x + y * 800] = rgb;
}

/* * * * *
 * 在屏幕上显示bmp格式图片，可调整大小、位置
 * 参数：
 *  fd: 文件描述符
 *  ffector: 缩放参数、影响图片实际显示大小。
 *  xmove: 图片在屏幕水平移动的距离
 *  ymove: 图片在屏幕垂直移动的距离
 * * * * */
void showbmp(int fd, double ffector, int xmove, int ymove)
{
    unsigned char c[4];
    int pix_offset;
    lseek(fd, 0xa, SEEK_SET);
    read(fd, c, 4);
    pix_offset = c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);

    int x;
    lseek(fd, 0x12, SEEK_SET);
    read(fd, c, 4);
    x = c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);

    int y;
    lseek(fd, 0x16, SEEK_SET);
    read(fd, c, 4);
    y = c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);

    int color_deepth;
    lseek(fd, 0x1c, SEEK_SET);
    read(fd, c, 2);
    color_deepth = c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);

    int endzero = 0;
    if (color_deepth == 24)
        endzero = (4 - x * 3 % 4) % 4;
    char *data = malloc(abs(x) * abs(y) * color_deepth / 8);
    lseek(fd, pix_offset, SEEK_SET);
    read(fd, data, x * y * color_deepth / 8);

    int w, h;
    int color;
    unsigned char *p = data;
    unsigned char a, r, g, b;
    double fectorx;
    double fectory;
    if (ffector > 32)
        ffector = 32;
    //fprintf(stderr, "jpg : Line %d ffector = %lf\n", __LINE__, ffector);
    if (x >= y)
    {
        fectorx = abs(x) / 800.0 * ffector;
        fectory = abs(y) / 480.0 * ffector;
    }
    else
    {
        fectorx = abs(x) / 480.0 * ffector;
        fectory = abs(y) / 800.0 * ffector;
    }
    int offset;

    for (h = 0; h < 480; h++)
    {
        for (w = 0; w < 800; w++)
        {
            offset = (int)((w + xmove) * fectorx) * color_deepth / 8 + (int)((h + ymove) * fectory) * abs(x) * color_deepth / 8 + endzero * (int)(h * fectory);
            if ((offset < abs(x) * abs(y) * color_deepth / 8) && offset >= 0) //w和h是否越界应该分开检查(这里没有)，否则会有小问题
            {
                p = data + offset;
                b = *p;
                g = *(p + 1);
                r = *(p + 2);
                a = (color_deepth == 24) ? 0 : *(p + 3);
                color = b | g << 8 | r << 16 | a << 24;
            }
            else
                color = 0xFFFFFF;
            drawpoint((x > 0) ? w : (799 - w), (y < 0) ? h : (479 - h), color);
            //drawpoint((x>0)?(799-w):w,(y<0)?(479-h):h,color);
        }
    }
    free(data);
}

/*int main(int argc, char **argv)
{
    
    int sfd = open("/dev/fb0",O_RDWR);
	if(sfd == -1)
	{
		perror("open device error");
		return -1;
	}
    
    struct fb_var_screeninfo buf;
    ioctl(sfd,FBIOGET_VSCREENINFO,&buf);
    fbuff = mmap(NULL, buf.xres * buf.yres * buf.bits_per_pixel / 8, PROT_READ | PROT_WRITE, MAP_SHARED, sfd,  0);
    
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
    }
    char c[4];
    void *ptr = c;
    
    read(fd, c, 2);
    if(c[0] == 0x42 && c[1] == 0x4d)
    {
        printf("It is a BMP file\n");
    }

    lseek(fd, 0x02, SEEK_SET);
    read(fd, c, 4);
    printf("BMP size = %d\n",*(int *)ptr);

    lseek(fd, 0x12, SEEK_SET);
    read(fd, c, 4);

    printf("BMP resolution = %d ",*(int *)ptr);
    read(fd, c, 4);
    printf("* %d\n",*(int *)ptr);

    lseek(fd, 0x1c, SEEK_SET);
    read(fd, c, 2);
    printf("BMP Depth/8bit = %d bit\n",*(int *)ptr);
    
    showbmp(fd);
    
    munmap(fbuf, buf.xres * buf.yres * buf.bits_per_pixel / 8);
    close(fd);
    close(sfd);
    return 0;
}*/